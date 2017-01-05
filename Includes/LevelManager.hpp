/** @file LevelManager.hpp
 * Load the levels from the level files and render them to the display.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_LEVEL_MANAGER_HPP
#define HPP_LEVEL_MANAGER_HPP

namespace LevelManager
{

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

}

#endif