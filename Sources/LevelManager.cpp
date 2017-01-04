/** @file LevelManager.cpp
 * @see LevelManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <LevelManager.hpp>
#include <Log.hpp>
#include <Texture.hpp>
#include <TextureManager.hpp>

namespace LevelManager
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All available blocks. */
typedef enum
{
	BLOCK_ID_RIVER_SAND,
	BLOCK_ID_GRASS,
	BLOCK_ID_WALL_STONE_1,
	BLOCK_ID_DIRT_1,
	BLOCK_ID_DIRT_2,
	BLOCK_IDS_COUNT
} BlockId;

/** A block. */
typedef struct
{
	Texture *pointerTexture; //!< The texture used to render the block.
	int isColliding; //!< Set to 1 if the block can't be crossed, set to 0 if player and moveable objects can walk throughout the block.
} Block;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The level width in blocks. */
static int _levelWidthBlocks;
/** The level height in blocks. */
static int _levelHeightBlocks;

/** The display width in blocks (rounded to the upper to make sure all the display is filled). */
static int _displayWidthBlocks;
/** The display height in blocks (rounded to the upper to make sure all the display is filled). */
static int _displayHeightBlocks;

/** Contain all existing blocks. */
static Block _blocks[BLOCK_IDS_COUNT];
/** Contain all level blocks. */
static Block *_pointerLevelBlocks[CONFIGURATION_LEVEL_MAXIMUM_WIDTH * CONFIGURATION_LEVEL_MAXIMUM_HEIGHT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// Create all blocks
	_blocks[BLOCK_ID_RIVER_SAND].pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_RIVER_SAND);
	_blocks[BLOCK_ID_RIVER_SAND].isColliding = 0;
	_blocks[BLOCK_ID_GRASS].pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_GREEN_GRASS);
	_blocks[BLOCK_ID_GRASS].isColliding = 0;
	_blocks[BLOCK_ID_WALL_STONE_1].pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_WALL_STONE_1);
	_blocks[BLOCK_ID_WALL_STONE_1].isColliding = 1;
	_blocks[BLOCK_ID_DIRT_1].pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_DIRT_1);
	_blocks[BLOCK_ID_DIRT_1].isColliding = 0;
	_blocks[BLOCK_ID_DIRT_2].pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_DIRT_2);
	_blocks[BLOCK_ID_DIRT_2].isColliding = 0;
	
	// Compute the amount of blocks that can be simultanously displayed on the current display
	_displayWidthBlocks = CONFIGURATION_DISPLAY_WIDTH / CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (CONFIGURATION_DISPLAY_WIDTH % CONFIGURATION_LEVEL_BLOCK_SIZE != 0) _displayWidthBlocks++;
	_displayHeightBlocks = CONFIGURATION_DISPLAY_HEIGHT / CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (CONFIGURATION_DISPLAY_HEIGHT % CONFIGURATION_LEVEL_BLOCK_SIZE != 0) _displayHeightBlocks++;
	LOG_DEBUG("Display size : %dx%d pixels, %dx%d blocks.\n", CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, _displayWidthBlocks, _displayHeightBlocks);

	return 0;
}

void uninitialize()
{
	// TODO if needed
}

int loadLevel(const char *sceneFileName, const char *objectsFileName)
{
	FILE *pointerFile;
	int x, y, character, i;
	BlockId blockId;
	
	// Try to open the scene file
	pointerFile = fopen(sceneFileName, "r");
	if (pointerFile == NULL)
	{
		LOG_ERROR("Could not open '%s' (%s).\n", sceneFileName, strerror(errno));
		return -1;
	}
	
	_levelWidthBlocks = 0;
	_levelHeightBlocks = 0;
	
	// Parse the whole file considering it does not contain errors
	i = 0;
	for (y = 0; y < CONFIGURATION_LEVEL_MAXIMUM_HEIGHT; y++)
	{
		for (x = 0; x < CONFIGURATION_LEVEL_MAXIMUM_WIDTH; x++)
		{
			// Read a block index
			if (fscanf(pointerFile, "%d", (int *) &blockId) != 1) goto Scene_Loading_End;
			if (blockId == -1)
			{
				LOG_ERROR("Block (%d, %d) has no defined texture index.\n", x, y);
				fclose(pointerFile);
				return -1;
			}
			
			// Set the corresponding block
			_pointerLevelBlocks[i] = &_blocks[blockId];
			i++;
			
			// Discard the following comma
			character = fgetc(pointerFile);
			// Load next row if a newline character is found
			if (character == '\n')
			{
				_levelWidthBlocks = x + 1; // Set level width in the same time
				break;
			}
		}
	}
	
Scene_Loading_End:
	_levelHeightBlocks = y;
	LOG_DEBUG("Level size = %dx%d blocks.\n", _levelWidthBlocks, _levelHeightBlocks);
	fclose(pointerFile);
	
	
	
	// TODO objects
	
	return 0;
}

void renderScene(int topLeftX, int topLeftY)
{
	int xDisplayBlock, xStartingBlock, yStartingBlock, yDisplayBlock, xBlock, yBlock, xDisplayBlocksCount, yDisplayBlocksCount, xStartingPixel, yStartingPixel, xPixel, yPixel;
	
	// Get the amount of pixels the rendering must be shifted about in the begining blocks
	xStartingPixel = -(topLeftX % CONFIGURATION_LEVEL_BLOCK_SIZE); // Invert result sign to make negative camera coordinates go to left and positive camera coordinates go to right
	yStartingPixel = -(topLeftY % CONFIGURATION_LEVEL_BLOCK_SIZE);
	
	// Convert pixel coordinates to blocks
	xStartingBlock = topLeftX / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yStartingBlock = topLeftY / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Compute the amount of blocks to display
	// TODO handle user reaching the rightmost or the downer side
	xDisplayBlocksCount = _displayWidthBlocks;
	if (xStartingPixel < 0) xDisplayBlocksCount++; // Display one more block on the right if the leftmost block is not fully displayed
	yDisplayBlocksCount = _displayHeightBlocks;
	if (yStartingPixel < 0) yDisplayBlocksCount++; // Display one more block on the bottom if the upper block is not fully displayed
	
	// Render a full display from the specified coordinates
	yPixel = yStartingPixel;
	for (yDisplayBlock = 0; yDisplayBlock < yDisplayBlocksCount; yDisplayBlock++)
	{
		xPixel = xStartingPixel;
		for (xDisplayBlock = 0; xDisplayBlock < xDisplayBlocksCount; xDisplayBlock++)
		{
			// Compute the level block coordinates
			xBlock = xStartingBlock + xDisplayBlock;
			yBlock = yStartingBlock + yDisplayBlock;
			
			// Render the block only if it existing in the level
			if ((xBlock >= 0) && (yBlock >= 0)) _pointerLevelBlocks[yBlock * _levelWidthBlocks + xBlock]->pointerTexture->render(xPixel, yPixel);
			
			xPixel += CONFIGURATION_LEVEL_BLOCK_SIZE;
		}
		yPixel += CONFIGURATION_LEVEL_BLOCK_SIZE;
	}
}

}
