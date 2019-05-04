/** @file LevelManager.cpp
 * @see LevelManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <cerrno>
#include <Configuration.hpp>
#include <cstdio>
#include <cstring>
#include <FightingEntityPlayer.hpp>
#include <FileManager.hpp>
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
	OBJECT_ID_LEVEL_EXIT,
	OBJECT_ID_GOLDEN_MEDIPACK,
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

/** Cache medipack texture. */
static Texture *_pointerMedipackTexture;
/** Cache golden medipack texture. */
static Texture *_pointerGoldenMedipackTexture;
/** Cache ammunition texture. */
static Texture *_pointerAmmunitionTexture;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
std::list<EnemySpawnerStaticEntity *> enemySpawnersList;

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
	LOG_DEBUG("Display size : %dx%d pixels, %dx%d blocks.", Renderer::displayWidth, Renderer::displayHeight, _displayWidthBlocks, _displayHeightBlocks);
	
	// Cache some values
	_pointerMedipackTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIPACK);
	_pointerGoldenMedipackTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_GOLDEN_MEDIPACK);
	_pointerAmmunitionTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_AMMUNITION);

	return 0;
}

void uninitialize()
{
	// TODO if needed
}

int loadLevel(int levelNumber)
{
	FILE *pointerFile;
	int x, y, character, i, objectId, isPlayerSpawned = 0;
	TextureManager::TextureId textureId;
	char stringFileName[256];
	
	LOG_DEBUG("Loading level %d...", levelNumber);
	
	// Try to open the scene file
	snprintf(stringFileName, sizeof(stringFileName), FileManager::getFilePath("Levels/%d_Scene.csv"), levelNumber);
	pointerFile = fopen(stringFileName, "r");
	if (pointerFile == NULL)
	{
		LOG_ERROR("Could not open '%s' (%s).", stringFileName, strerror(errno));
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
			if (fscanf(pointerFile, "%d", (int *) &textureId) != 1) goto Scene_Loading_End;
			
			// Set block texture
			_levelBlocks[i].pointerTexture = TextureManager::getTextureFromId(textureId);
			
			// Set block collision
			if (textureId < TextureManager::TEXTURE_ID_SCENE_WALL_0) _levelBlocks[i].content = 0; // This is a floor block, it does not collide with nothing
			else if (textureId < TextureManager::TEXTURE_ID_MEDIPACK) _levelBlocks[i].content = BLOCK_CONTENT_WALL; // This is a wall block
			else
			{
				LOG_ERROR("Block (%d, %d) ID is bad : %d.", x, y, (int) textureId);
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
	LOG_DEBUG("Level size : %dx%d blocks.", _levelWidthBlocks, _levelHeightBlocks);
	fclose(pointerFile);
	
	// Try to open the objects file
	snprintf(stringFileName, sizeof(stringFileName), FileManager::getFilePath("Levels/%d_Objects.csv"), levelNumber);
	pointerFile = fopen(stringFileName, "r");
	if (pointerFile == NULL)
	{
		LOG_ERROR("Could not open '%s' (%s).", stringFileName, strerror(errno));
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
				LOG_ERROR("Failed to read the object (%d, %d) value. Make sure the objects file has the same dimensions than the scene file.", x, y);
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
						LOG_ERROR("More than one player are present on the map. Make sure to have only one player.");
						goto Objects_Loading_Error;
					}
					else
					{
						// Get player size from its texture
						Texture *pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_UP);
						int playerWidth = pointerTexture->getWidth();
						int playerHeight = pointerTexture->getHeight();
						
						// Put the player at the block center
						pointerPlayer->setX((x * CONFIGURATION_LEVEL_BLOCK_SIZE) + ((CONFIGURATION_LEVEL_BLOCK_SIZE - playerWidth) / 2));
						pointerPlayer->setY((y * CONFIGURATION_LEVEL_BLOCK_SIZE) + ((CONFIGURATION_LEVEL_BLOCK_SIZE - playerHeight) / 2));
						isPlayerSpawned = 1;
						LOG_DEBUG("Spawned player on block (%d, %d).", x, y);
					}
					break;
					
				case OBJECT_ID_MEDIPACK:
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_MEDIPACK;
					LOG_DEBUG("Spawned medipack on block (%d, %d).", x, y);
					break;
					
				case OBJECT_ID_GOLDEN_MEDIPACK:
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_GOLDEN_MEDIPACK;
					LOG_DEBUG("Spawned golden medipack on block (%d, %d).", x, y);
					break;
					
				case OBJECT_ID_AMMUNITION:
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_AMMUNITION;
					LOG_DEBUG("Spawned ammunition on block (%d, %d).", x, y);
					break;
					
				case OBJECT_ID_ENEMY_SPAWNER:
					enemySpawnersList.push_front(new EnemySpawnerStaticEntity(x * CONFIGURATION_LEVEL_BLOCK_SIZE, y * CONFIGURATION_LEVEL_BLOCK_SIZE));
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_ENEMY_SPAWNER;
					LOG_DEBUG("Spawned enemy spawner on block (%d, %d).", x, y);
					break;
					
				case OBJECT_ID_LEVEL_EXIT:
					_levelBlocks[COMPUTE_BLOCK_INDEX(x, y)].content |= BLOCK_CONTENT_LEVEL_EXIT;
					LOG_DEBUG("Spawned level exit on block (%d, %d).", x, y);
					break;
					
				default:
					LOG_INFORMATION("Unhandled object (object ID : %d) at block (%d, %d).", objectId, x, y);
					break;
			}
			
			// Discard the following comma
			character = fgetc(pointerFile);
		}
	}
	
	// Make sure there is a player
	if (!isPlayerSpawned)
	{
		LOG_ERROR("Map does not contain any player.");
		goto Objects_Loading_Error;
	}
	
	fclose(pointerFile);
	
	LOG_INFORMATION("Level %d successfully loaded.", levelNumber);
	return 0;
	
Objects_Loading_Error:
	fclose(pointerFile);
	return -1;
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
				if (pointerBlock->content & BLOCK_CONTENT_MEDIPACK) _pointerMedipackTexture->render(xPixel, yPixel);
				else if (pointerBlock->content & BLOCK_CONTENT_GOLDEN_MEDIPACK) _pointerGoldenMedipackTexture->render(xPixel, yPixel);
				else if (pointerBlock->content & BLOCK_CONTENT_AMMUNITION) _pointerAmmunitionTexture->render(xPixel, yPixel);
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

int isObstaclePresentOnVerticalAxis(int topmostY, int downerY, int x)
{
	// Convert coordinates to blocks
	topmostY /= CONFIGURATION_LEVEL_BLOCK_SIZE;
	downerY /= CONFIGURATION_LEVEL_BLOCK_SIZE;
	x /= CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Make some coordinate checks
	assert(topmostY >= 0);
	assert(topmostY < _levelHeightBlocks);
	assert(downerY >= 0);
	assert(downerY < _levelHeightBlocks);
	assert(x >= 0);
	assert(x < _levelWidthBlocks);
	
	// Check all blocks along between the specified coordinates
	while (topmostY < downerY)
	{
		if (_levelBlocks[COMPUTE_BLOCK_INDEX(x, topmostY)].content & (BLOCK_CONTENT_WALL | BLOCK_CONTENT_ENEMY_SPAWNER)) return 1;
		topmostY++;
	}
	
	return 0;
}

int isObstaclePresentOnHorizontalAxis(int leftmostX, int rightmostX, int y)
{
	// Convert coordinates to blocks
	leftmostX /= CONFIGURATION_LEVEL_BLOCK_SIZE;
	rightmostX /= CONFIGURATION_LEVEL_BLOCK_SIZE;
	y /= CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Make some coordinate checks
	assert(leftmostX >= 0);
	assert(leftmostX < _levelWidthBlocks);
	assert(rightmostX >= 0);
	assert(rightmostX < _levelWidthBlocks);
	assert(y >= 0);
	assert(y < _levelHeightBlocks);
	
	// Check all blocks along between the specified coordinates
	while (leftmostX < rightmostX)
	{
		if (_levelBlocks[COMPUTE_BLOCK_INDEX(leftmostX, y)].content & (BLOCK_CONTENT_WALL | BLOCK_CONTENT_ENEMY_SPAWNER)) return 1;
		leftmostX++;
	}
	
	return 0;
}

int getBlockContent(int x, int y)
{
	int xBlock, yBlock;
	
	// Convert coordinates to blocks
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Make sure the block is existing
	assert(xBlock >= 0);
	assert(xBlock < _levelWidthBlocks);
	assert(yBlock >= 0);
	assert(yBlock < _levelHeightBlocks);
	
	return _levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content;
}

void setBlockContent(int x, int y, int content)
{
	int xBlock, yBlock;
	
	// Convert coordinates to blocks
	xBlock = x / CONFIGURATION_LEVEL_BLOCK_SIZE;
	yBlock = y / CONFIGURATION_LEVEL_BLOCK_SIZE;
	
	// Make sure the block is existing
	assert(xBlock >= 0);
	assert(xBlock < _levelWidthBlocks);
	assert(yBlock >= 0);
	assert(yBlock < _levelHeightBlocks);
	
	_levelBlocks[COMPUTE_BLOCK_INDEX(xBlock, yBlock)].content = content;
}

}
