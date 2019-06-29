/** @file LevelManager.hpp
 * Load the levels from the level files and render them to the display.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_LEVEL_MANAGER_HPP
#define HPP_LEVEL_MANAGER_HPP

#include <EnemySpawnerStaticEntity.hpp>
#include <list>

namespace LevelManager
{
	/** All available bits to use in the content bit field of a block. */
	typedef enum
	{
		BLOCK_CONTENT_WALL = 1 << 0, //!< If set, tell that this block is a scene wall, so no entity can cross it.
		BLOCK_CONTENT_MEDIPACK = 1 << 1, //!< If set, tell that a medipack is present on top of this block.
		BLOCK_CONTENT_GOLDEN_MEDIPACK = 1 << 2, //!< If set, tell that a golden medipack is present on top of this block.
		BLOCK_CONTENT_AMMUNITION = 1 << 3, //!< If set, tell that ammunition are present on top of this block.
		BLOCK_CONTENT_MACHINE_GUN_BONUS = 1 << 4, //!< If set, tell that a machine gun bonus is present on top of this block.
		BLOCK_CONTENT_BULLETPROOF_VEST_BONUS = 1 << 5, //!< If set, tell that a bulletproof vest bonus is present on top of this block.
		BLOCK_CONTENT_ENEMY = 1 << 6, //!< If set, tell that an enemy is walking on this block, so no other enemy can go through this block to avoid collisions.
		BLOCK_CONTENT_ENEMY_SPAWNER = 1 << 7, //!< If set, tell that the whole block is occupied by an enemy spawner. Player and enemies can't cross the block, but bullets can.
		BLOCK_CONTENT_LEVEL_EXIT = 1 << 8 //!< If set and the player walks on it, the game will load the next level.
	} BlockContent;

	/** Contain all living enemy spawners. */
	extern std::list<EnemySpawnerStaticEntity *> enemySpawnersList;

	/** Prepare all scene blocks.
	 * @return -1 if an error occurred,
	 * @return 0 if the level manager is successfully initialized.
	 */
	int initialize();

	/** Free all allocated resources. */
	void uninitialize();

	/** Load a level from two Comma Separated Value files (x_Scene.csv and x_Objects.csv, where x is the level number).
	 * @param levelNumber The level number (starting from 0).
	 * @return -1 if an error occurred,
	 * @return 0 if the level was successfully loaded.
	 */
	int loadLevel(int levelNumber);

	/** Display the scene (decor) to the main renderer.
	 * @param topLeftX This is the scene horizontal coordinate to start drawing from.
	 * @param topLeftY This is the scene vertical coordinate to start drawing from.
	 */
	void renderScene(int topLeftX, int topLeftY);

	/** Compute the distance in pixels separating the specified point from the upper block.
	 * @param x Point x coordinate in pixels.
	 * @param y Point y coordinate in pixels.
	 * @param blockContent The type of block to check (wall, enemy spawner...). Block types can be ORed to check multiple blocks at a time.
	 * @return The distance in pixels. To be faster, the functions stops if the first searched block is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
	 */
	int getDistanceFromUpperBlock(int x, int y, int blockContent);

	/** Compute the distance in pixels separating the specified point from the downer block.
	 * @param x Point x coordinate in pixels.
	 * @param y Point y coordinate in pixels.
	 * @param blockContent The type of block to check (wall, enemy spawner...). Block types can be ORed to check multiple blocks at a time.
	 * @return The distance in pixels. To be faster, the functions stops if the first searched block is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
	 */
	int getDistanceFromDownerBlock(int x, int y, int blockContent);

	/** Compute the distance in pixels separating the specified point from the leftmost block.
	 * @param x Point x coordinate in pixels.
	 * @param y Point y coordinate in pixels.
	 * @param blockContent The type of block to check (wall, enemy spawner...). Block types can be ORed to check multiple blocks at a time.
	 * @return The distance in pixels. To be faster, the functions stops if the first searched block is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
	 */
	int getDistanceFromLeftmostBlock(int x, int y, int blockContent);

	/** Compute the distance in pixels separating the specified point from the rightmost block.
	 * @param x Point x coordinate in pixels.
	 * @param y Point y coordinate in pixels.
	 * @param blockContent The type of block to check (wall, enemy spawner...). Block types can be ORed to check multiple blocks at a time.
	 * @return The distance in pixels. To be faster, the functions stops if the first searched block is further than CONFIGURATION_LEVEL_BLOCK_SIZE pixels.
	 */
	int getDistanceFromRightmostBlock(int x, int y, int blockContent);

	/** Tell if there is an obstacle (wall, enemy spawner...) on the vertical axis between two specified points.
	 * @param topmostY The topmost point Y coordinate.
	 * @param downerY The downer point Y coordinate.
	 * @param x The horizontal coordinate, which is the same for the two points.
	 * @return false if there is no obstacle,
	 * @return true if there is one or more obstacles.
	 */
	bool isObstaclePresentOnVerticalAxis(int topmostY, int downerY, int x);

	/** Tell if there is an obstacle (wall, enemy spawner...) on the horizontal axis between two specified points.
	 * @param leftmostX The leftmost point X coordinate.
	 * @param rightmostX The rightmost point X coordinate.
	 * @param y The vertical coordinate, which is the same for the two points.
	 * @return false if there is no obstacle,
	 * @return true if there is one or more obstacles.
	 */
	bool isObstaclePresentOnHorizontalAxis(int leftmostX, int rightmostX, int y);

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
	
	/** Spawn a random item (or nothing) on the block into which coordinates are contained.
	 * @param x X coordinate in pixels.
	 * @param y Y coordinate in pixels.
	 */
	void spawnItem(int x, int y);
}

#endif
