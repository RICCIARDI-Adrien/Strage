/** @file TextureManager.hpp
 * Load and provide to other subsystems all game textures.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_TEXTURE_MANAGER_HPP
#define HPP_TEXTURE_MANAGER_HPP

namespace TextureManager // TODO is TextureManager useful ?
{

/** All available tile textures. */
typedef enum
{
	TILE_ID_RIVER_SAND,
	TILE_ID_RIVER_GREEN_GRASS,
	TILE_ID_RIVER_WALL_GREY_STONES,
	TILE_ID_DIRT_1,
	TILE_ID_DIRT_2,
	TILE_IDS_COUNT
} TileId;

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

}

#endif