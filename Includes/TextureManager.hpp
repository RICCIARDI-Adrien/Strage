/** @file TextureManager.hpp
 * Load and provide to other subsystems all game textures.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_TEXTURE_MANAGER_HPP
#define HPP_TEXTURE_MANAGER_HPP

#include <Texture.hpp>

namespace TextureManager
{

/** All available textures. */
typedef enum
{
	// Scene tiles
	TEXTURE_ID_RIVER_SAND,
	TEXTURE_ID_GREEN_GRASS,
	TEXTURE_ID_WALL_STONE_1,
	TEXTURE_ID_DIRT_1,
	TEXTURE_ID_DIRT_2,
	
	// Static objects
	TEXTURE_ID_MEDIPACK,
	TEXTURE_ID_AMMUNITION,
	TEXTURE_ID_ENEMY_SPAWNER,
	
	// Moving objects
	TEXTURE_ID_PLAYER,
	TEXTURE_ID_ENEMY,
	TEXTURE_ID_BULLET,
	
	// Animated textures
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_0,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_1,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_2,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_3,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_4,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_5,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_6,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_7,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_8,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_9,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_10,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_11,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_12,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_13,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_14,
	TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_15,
	TEXTURE_ID_SHIP_EXPLOSION_0,
	TEXTURE_ID_SHIP_EXPLOSION_1,
	TEXTURE_ID_SHIP_EXPLOSION_2,
	TEXTURE_ID_SHIP_EXPLOSION_3,
	TEXTURE_ID_SHIP_EXPLOSION_4,
	TEXTURE_ID_SHIP_EXPLOSION_5,
	TEXTURE_ID_SHIP_EXPLOSION_6,
	TEXTURE_ID_SHIP_EXPLOSION_7,
	TEXTURE_ID_SHIP_EXPLOSION_8,
	TEXTURE_ID_SHIP_EXPLOSION_9,
	TEXTURE_ID_SHIP_EXPLOSION_10,
	TEXTURE_ID_SHIP_EXPLOSION_11,
	TEXTURE_ID_SHIP_EXPLOSION_12,
	TEXTURE_ID_SHIP_EXPLOSION_13,
	TEXTURE_ID_SHIP_EXPLOSION_14,
	TEXTURE_ID_SHIP_EXPLOSION_15,
	TEXTURE_IDS_COUNT
} TextureId;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load all game textures.
 * @return 0 if all textures were successfully initialized,
 * @return -1 if an error occurred.
 */
int initialize();

/** Free all allocated resources. */
void uninitialize();

/** Retrieve a texture.
 * @param id The texture identifier.
 * @return The texture or NULL if the provided ID is bad.
 */
Texture *getTextureFromId(TextureId id);

}

#endif