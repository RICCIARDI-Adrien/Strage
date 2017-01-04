/** @file LevelManager.cpp
 * @see LevelManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstdio>
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
	LOG("Information : display width = %d pixels, %d blocks.\n", CONFIGURATION_DISPLAY_WIDTH, _displayWidthBlocks);
	_displayHeightBlocks = CONFIGURATION_DISPLAY_HEIGHT / CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (CONFIGURATION_DISPLAY_HEIGHT % CONFIGURATION_LEVEL_BLOCK_SIZE != 0) _displayHeightBlocks++;
	LOG("Information : display height = %d pixels, %d blocks.\n", CONFIGURATION_DISPLAY_HEIGHT, _displayHeightBlocks);
	
	// TEST
	_levelWidthBlocks = 8;
	_levelHeightBlocks = 5;
	for (int i = 0; i < 256*256; i++) _pointerLevelBlocks[i] = &_blocks[BLOCK_ID_RIVER_SAND];
	_pointerLevelBlocks[6] = &_blocks[BLOCK_ID_GRASS];
	
	return 0;
}

void uninitialize()
{
	// TODO if needed
}

/*int loadLevel(const char *sceneFileName, const char *objectsFileName)
{
}*/

void renderScene(int topLeftX, int topLeftY)
{
	int yBlock, xBlock;
	
	// Render a full display from the specified coordinates
	// TODO do not use _displayHeightBlocks but a variable (when the user reaches the rightmost or the downer side)
	for (yBlock = 0; yBlock < _displayHeightBlocks; yBlock++)
	{
		for (xBlock = 0; xBlock < _displayHeightBlocks; xBlock++)
		{
			_pointerLevelBlocks[yBlock * _displayWidthBlocks + xBlock]->pointerTexture->render(xBlock * 64, yBlock * 64);
			
		}
	}
}

}
