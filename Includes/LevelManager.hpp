/** @file LevelManager.hpp
 * Load the levels from the level files and render them to the display.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_LEVEL_MANAGER_HPP
#define HPP_LEVEL_MANAGER_HPP

namespace LevelManager
{

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All available bits to use in the content bit field of a block. */
typedef enum
{
	BLOCK_CONTENT_WALL = 1 << 0, //!< If set, tell that this block is a scene wall, so no entity can cross it.
	BLOCK_CONTENT_MEDIPACK = 1 << 1, //!< If set, tell that a medipack is present on top of this block.
	BLOCK_CONTENT_AMMUNITION = 1 << 2, //!< If set, tell that ammunition are present on top of this block.
	BLOCK_CONTENT_ENEMY = 1 << 3 //!< If set, tell that an enemy is walking on this block, so no other enemy can go through this block to avoid collisions.
} BlockContent;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Prepare all scene blocks.
 * @return 0 if the level manager is successfully initialized,
 * @return -1 if an error occurred.
 */
int initialize();

/** Free all allocated resources. */
void uninitialize();

/** Load a level.
 * @param sceneFileName The Comma Separated Value file describing the map decor.
 * @param objectsFileName The Comma Separated Value file telling where objects are located.
 * @return 0 if the level was successfully loaded,
 * @return -1 if an error occurred.
 */
int loadLevel(const char *sceneFileName, const char *objectsFileName);

/** Display the scene (decor) to the main renderer.
 * @param topLeftX This is the scene horizontal coordinate to start drawing from.
 * @param topLeftY This is the scene vertical coordinate to start drawing from.
 */
void renderScene(int topLeftX, int topLeftY);

/** Compute the distance in pixels separating the specified point from the upper wall.
 * @param x Point x coordinate.
 * @param y Point y coordinate.
 * @return The distance in pixels. To be faster, the functions stops if the first wall is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
 */
int getDistanceFromUpperWall(int x, int y);

/** Compute the distance in pixels separating the specified point from the downer wall.
 * @param x Point x coordinate.
 * @param y Point y coordinate.
 * @return The distance in pixels. To be faster, the functions stops if the first wall is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
 */
int getDistanceFromDownerWall(int x, int y);

/** Compute the distance in pixels separating the specified point from the leftmost wall.
 * @param x Point x coordinate.
 * @param y Point y coordinate.
 * @return The distance in pixels. To be faster, the functions stops if the first wall is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
 */
int getDistanceFromLeftmostWall(int x, int y);

/** Compute the distance in pixels separating the specified point from the rightmost wall.
 * @param x Point x coordinate.
 * @param y Point y coordinate.
 * @return The distance in pixels. To be faster, the functions stops if the first wall is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
 */
int getDistanceFromRightmostWall(int x, int y);

/** Get a block content.
 * @param x X coordinate in pixels.
 * @param y Y coordinate in pixels.
 * @return The block content, or 0 if the provided coordinates were bad.
 */
int getBlockContent(int x, int y);

/** Set a block content.
 * @param x X coordinate in pixels.
 * @param y Y coordinate in pixels.
 * @param content The new block content, fully overwriting the previous one. Use values from BlockContent to fill it.
 */
void setBlockContent(int x, int y, int content);

}

#endif