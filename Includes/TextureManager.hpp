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
	
	// Pickable objects
	TEXTURE_ID_MEDIPACK,
	
	// Moving objects
	TEXTURE_ID_PLAYER,
	TEXTURE_ID_BULLET,
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