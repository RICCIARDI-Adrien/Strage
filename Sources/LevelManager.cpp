/** @file LevelManager.cpp
 * @see LevelManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <EntityEnemySpawner.hpp>
#include <FightingEntityPlayer.hpp>
#include <LevelManager.hpp>
#include <list>
#include <Log.hpp>
#include <Texture.hpp>
#include <TextureManager.hpp>

namespace LevelManager
{

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Compute a block index according to its coordinates.
 * @param x X coordinate (in blocks).
 * @param y Y coordinate (in blocks).
 * @return The corresponding index.
 */
#define COMPUTE_BLOCK_INDEX(x, y) (((y) * _levelWidthBlocks) + (x))

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
	int content; //!< A bit field telling what the block contains (objects or scene details). Use values from BlockContent to handle the bit field.
} Block;

/** All available spawnable objects. */
typedef enum
{
	OBJECT_ID_PLAYER,
	OBJECT_ID_MEDIPACK,
	OBJECT_ID_AMMUNITION,
	OBJECT_ID_ENEMY_SPAWNER,
	OBJECT_IDS_COUNT
} ObjectId;

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

/** Contain all level blocks. */
static Block _levelBlocks[CONFIGURATION_LEVEL_MAXIMUM_WIDTH * CONFIGURATION_LEVEL_MAXIMUM_HEIGHT];

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
std::list<EntityEnemySpawner *> enemySpawnersList;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Set a block texture and wall collision.
 * @param blockId The block identifier.
 * @param pointerBlock On output, fill this block with the right values.
 * @return 0 if the block was successfully set,
 * @return -1 if the provided block ID is unknown.
 */
static int _setBlockFromId(BlockId blockId, Block *pointerBlock)
{
	TextureManager::TextureId textureId;
	
	switch (blockId)
	{
		case BLOCK_ID_RIVER_SAND:
			textureId = TextureManager::TEXTURE_ID_RIVER_SAND;
			pointerBlock->content = 0;
			break;
			
		case BLOCK_ID_GRASS:
			textureId = TextureManager::TEXTURE_ID_GREEN_GRASS;
			pointerBlock->content = 0;
			break;
			
		case BLOCK_ID_WALL_STONE_1:
			textureId = TextureManager::TEXTURE_ID_WALL_STONE_1;
			pointerBlock->content = BLOCK_CONTENT_WALL;
			break;
			
		case BLOCK_ID_DIRT_1:
			textureId = TextureManager::TEXTURE_ID_DIRT_1;
			pointerBlock->content = 0;
			break;
			
		case BLOCK_ID_DIRT_2:
			textureId = TextureManager::TEXTURE_ID_DIRT_2;
			pointerBlock->content = 0;
			break;
			
		default:
			return -1;
	}
	
	// Set block texture
	pointerBlock->pointerTexture = TextureManager::getTextureFromId(textureId);
	
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// Compute the amount of blocks that can be simultaneously displayed on the current display
	_displayWidthBlocks = Renderer::displayWidth / CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (Renderer::displayWidth % CONFIGURATION_LEVEL_BLOCK_SIZE != 0) _displayWidthBlocks++;
	_displayHeightBlocks = Renderer::displayHeight / CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (Renderer::displayHeight % CONFIGURATION_LEVEL_BLOCK_SIZE != 0) _displayHeightBlocks++;
	LOG_DEBUG("Display size : %dx%d pixels, %dx%d blocks.\n", Renderer::displayWidth, Renderer::displayHeight, _displayWidthBlocks, _displayHeightBlocks);

	return 0;
}

void uninitialize()
{
	// TODO if needed
}

int loadLevel(const char *sceneFileName, const char *objectsFileName)
{
	FILE *pointerFile;
	int x, y, character, i, blockId, objectId, isPlayerSpawned = 0;
	
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
			if (fscanf(pointerFile, "%d", &blockId) != 1) goto Scene_Loading_End;
			
			// Set the corresponding block texture and wall collision
			if (_setBlockFromId((BlockId) blockId, &_levelBlocks[i]) != 0)
			{
				LOG_ERROR("Block (%d, %d) ID is bad : %d.\n", x, y, blockId);
				fclose(pointerFile);
				return -1;
			}
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
	LOG_DEBUG("Level size : %dx%d blocks.\n", _levelWidthBlocks, _levelHeightBlocks);
	fclose(pointerFile);
	
	// Try to open the objects file
	pointerFile = fopen(objectsFileName, "r");
	if (pointerFile == NULL)
	{
		LOG_ERROR("Could not open '%s' (%s).\n", objectsFileName, strerror(errno));
		return -1;
	}
	
	// Spawn objects
	for (y = 0; y < _levelHeightBlocks; y++)
	{
		for (x = 0; x < _levelWidthBlocks; x++)
		{
			// Read an object index
			if (fscanf(pointerFile, "%d", &objectId) != 1)
			{
				LOG_ERROR("Failed to read the object (%d, %d) value. Make sure the objects file has the same dimensions than the scene file.\n", x, y);
				goto Objects_Loading_Error;
			}
			
			// Spawn the requested object
			switch (objectId)
			{
				// Ignore unset blocks
				case -1:
					break;
				
				case OBJECT_ID_PLAYER:
					// Make sure the player is unique
					if (isPlayerSpawned)
					{
						LOG_ERROR("More than one player are present on the map. Make sure to have only one player.\n");
						goto Objects_Loading_Error;
					}
					else
					{
						// Get player size from its texture
						Texture *pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER);
						int playerWidth = pointerTexture->getWidth();
						int playerHeight = pointerTexture->getHeight();
						
						// Spawn the player at the block center
						pointerPlayer = new FightingEntityPlayer((x * CONFIGURATION_LEVEL_BLOCK_SIZE) + ((CONFIGURATION_LEVEL_BLOCK_SIZE - playerWidth) / 2), (y * CONFIGURATION_LEVEL_BLOCK_SIZE) + ((CONFIGURATION_LEVEL_BLOCK_SIZE - playerHeight) / 2));
						isPlayerSpawned = 1;
						LOG_DEBUG("Spawned player on block (%d, %d).\n", x, y);
					}
					break;
					
				case OBJECT_ID_MEDIPACK:
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_MEDIPACK;
					LOG_DEBUG("Spawned medipack on block (%d, %d).\n", x, y);
					break;
					
				case OBJECT_ID_AMMUNITION:
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_AMMUNITION;
					LOG_DEBUG("Spawned ammunition on block (%d, %d).\n", x, y);
					break;
					
				case OBJECT_ID_ENEMY_SPAWNER:
					enemySpawnersList.push_front(new EntityEnemySpawner(x * CONFIGURATION_LEVEL_BLOCK_SIZE, y * CONFIGURATION_LEVEL_BLOCK_SIZE));
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_ENEMY_SPAWNER;
					LOG_DEBUG("Spawned enemy spawner on block (%d, %d).\n", x, y);
					break;
					
				default:
					LOG_INFORMATION("Unhandled object (object ID : %d) at block (%d, %d).\n", objectId, x, y);
					break;
			}
			
			// Discard the following comma
			character = fgetc(pointerFile);
		}
	}
	
	// Make sure there is a player
	if (!isPlayerSpawned)
	{
		LOG_ERROR("Map does not contain any player.\n");
		goto Objects_Loading_Error;
	}
	
	fclose(pointerFile);
	
	LOG_INFORMATION("Level '%s' successfully loaded.\n", sceneFileName);
	return 0;
	
Objects_Loading_Error:
	fclose(pointerFile);
	return -1;
}

// TODO clean a loaded level to allow a new level to be loaded
void unloadLevel()
{
	// TODO clean lists
}

void renderScene(int topLeftX, int topLeftY)
{
	int xDisplayBlock, xStartingBlock, yStartingBlock, yDisplayBlock, xBlock, yBlock, xDisplayBlocksCount, yDisplayBlocksCount, xStartingPixel, yStartingPixel, xPixel, yPixel;
	Block *pointerBlock;
	
	// Get the amount of pixels the rendering must be shifted about in the beginning blocks
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
			
			// Render the block only if it is existing in the level
			if ((xBlock >= 0) && (yBlock >= 0) && (xBlock < _levelWidthBlocks) && (yBlock < _levelHeightBlocks))
			{
				// Display the block texture
				pointerBlock = &_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)]; // Compute the block index only once
				pointerBlock->pointerTexture->render(xPixel, yPixel);
				
				// Display an eventual item which can be on the block
				if (pointerBlock->content & BLOCK_CONTENT_MEDIPACK) TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIPACK)->render(xPixel, yPixel);
				else if (pointerBlock->content & BLOCK_CONTENT_AMMUNITION) TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_AMMUNITION)->render(xPixel, yPixel);
			}
			
			xPixel += CONFIGURATION_LEVEL_BLOCK_SIZE;
		}
		yPixel += CONFIGURATION_LEVEL_BLOCK_SIZE;
	}
}

int getDistanceFromUpperBlock(int x, int y, int blockContent)
{
	int xBlock, yBlock;
	
	// Convert to block coordinates
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Tell that the walls are close if the requested coordinates are (out of) level bounds
	if ((xBlock <= 0) || (xBlock >= _levelWidthBlocks - 1) || (yBlock <= 0) || (yBlock >= _levelWidthBlocks - 1)) return 0;
	
	// Is this block a wall ?
	if (_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content & blockContent) return 0;
	
	// Is upper block part of the floor ?
	if (!(_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock - 1)].content & blockContent)) return CONFIGURATION_LEVEL_BLOCK_SIZE; // Do not check further to optimize function speed
	
	// The upper block is a wall, compute the amount of pixels separating the provided coordinates from the wall
	return y % CONFIGURATION_LEVEL_BLOCK_SIZE;
}

int getDistanceFromDownerBlock(int x, int y, int blockContent)
{
	int xBlock, yBlock;
	
	// Convert to block coordinates
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Tell that the walls are close if the requested coordinates are (out of) level bounds
	if ((xBlock <= 0) || (xBlock >= _levelWidthBlocks - 1) || (yBlock <= 0) || (yBlock >= _levelWidthBlocks - 1)) return 0;
	
	// Is this block a wall ?
	if (_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content & blockContent) return 0;
	
	// Is upper block part of the floor ?
	if (!(_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock + 1)].content & blockContent)) return CONFIGURATION_LEVEL_BLOCK_SIZE; // Do not check further to optimize function speed
	
	// The upper block is a wall, compute the amount of pixels separating the provided coordinates from the wall
	return CONFIGURATION_LEVEL_BLOCK_SIZE - (y % CONFIGURATION_LEVEL_BLOCK_SIZE);
}

int getDistanceFromLeftmostBlock(int x, int y, int blockContent)
{
	int xBlock, yBlock;
	
	// Convert to block coordinates
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Tell that the walls are close if the requested coordinates are (out of) level bounds
	if ((xBlock <= 0) || (xBlock >= _levelWidthBlocks - 1) || (yBlock <= 0) || (yBlock >= _levelWidthBlocks - 1)) return 0;
	
	// Is this block a wall ?
	if (_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content & blockContent) return 0;
	
	// Is upper block part of the floor ?
	if (!(_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock - 1, yBlock)].content & blockContent)) return CONFIGURATION_LEVEL_BLOCK_SIZE; // Do not check further to optimize function speed
	
	// The upper block is a wall, compute the amount of pixels separating the provided coordinates from the wall
	return x % CONFIGURATION_LEVEL_BLOCK_SIZE;
}

int getDistanceFromRightmostBlock(int x, int y, int blockContent)
{
	int xBlock, yBlock;
	
	// Convert to block coordinates
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Tell that the walls are close if the requested coordinates are (out of) level bounds
	if ((xBlock <= 0) || (xBlock >= _levelWidthBlocks - 1) || (yBlock <= 0) || (yBlock >= _levelWidthBlocks - 1)) return 0;
	
	// Is this block a wall ?
	if (_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content & blockContent) return 0;
	
	// Is upper block part of the floor ?
	if (!(_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock + 1, yBlock)].content & blockContent)) return CONFIGURATION_LEVEL_BLOCK_SIZE; // Do not check further to optimize function speed
	
	// The upper block is a wall, compute the amount of pixels separating the provided coordinates from the wall
	return CONFIGURATION_LEVEL_BLOCK_SIZE - (x % CONFIGURATION_LEVEL_BLOCK_SIZE);
}

int getBlockContent(int x, int y)
{
	int xBlock, yBlock;
	
	// Convert coordinates to blocks
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	if ((xBlock < 0) || (xBlock >= _levelWidthBlocks) || (yBlock < 0) || (yBlock >= _levelHeightBlocks))
	{
		LOG_INFORMATION("Bad block pixel coordinates (%d, %d).\n", x, y);
		return 0;
	}
	
	return _levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content;
}

void setBlockContent(int x, int y, int content)
{
	int xBlock, yBlock;
	
	// Convert coordinates to blocks
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	if ((xBlock < 0) || (xBlock >= _levelWidthBlocks) || (yBlock < 0) || (yBlock >= _levelHeightBlocks))
	{
		LOG_INFORMATION("Bad block pixel coordinates (%d, %d).\n", x, y);
		return;
	}
	
	_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content = content;
}

}
