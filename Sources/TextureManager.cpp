/** @file TextureManager.cpp
 * @see TextureManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.hpp>
#include <cstdio>
#include <Log.hpp>
#include <Texture.hpp>
#include <TextureDisplayOverlay.hpp>
#include <TextureManager.hpp>

namespace TextureManager
{

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All tiles. */
static Texture *_pointerTextureManagerTextures[TEXTURE_IDS_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// TODO load with array of struct { string, int }?
	// Scene floors
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_0] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Clay_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_1] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Dirt_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_2] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Dirt_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_3] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Grass_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_4] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Grass_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_5] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Gravel_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_6] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Gravel_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_7] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Grid_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_8] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Level_Exit.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_9] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Plate_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_10] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Plate_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_11] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Sand_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_12] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Snow_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_13] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Stone_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_14] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Stone_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_15] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Stone_2.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_16] = new Texture(CONFIGURATION_PATH_TEXTURES "/Floor_Stone_3.bmp", 0);
	
	// Scene walls
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_0] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Stone_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_1] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Stone_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_2] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Stone_2.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_3] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Stone_3.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_4] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Stone_4.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_6] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Bricks_0.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_7] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Bricks_1.bmp", 0);
	
	// Static objects
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medipack.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK] = new Texture(CONFIGURATION_PATH_TEXTURES "/Golden_Medipack.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_AMMUNITION] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ammunition.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner.bmp", 1);
	
	// Moving objects
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Up.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Down.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Left.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Right.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Up.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Down.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Left.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Right.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Up.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Down.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Left.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Right.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Up.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Down.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Left.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Right.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Up.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Down.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Left.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Right.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Up.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Down.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Left.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Right.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Up.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Down.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Left.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Right.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Up.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_DOWN] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Down.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_LEFT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Left.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_RIGHT] = new Texture(CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Right.bmp", 0);
	
	// Animated textures
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_0] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_0.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_1] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_1.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_2] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_2.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_3] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_3.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_4] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_4.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_5] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_5.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_6] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_6.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_7] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_7.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_8] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_8.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_9] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_9.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_10] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_10.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_11] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_11.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_12] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_12.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_13] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_13.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_14] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_14.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_15] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion_15.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_0] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_0.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_1] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_1.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_2] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_2.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_3] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_3.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_4] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_4.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_5] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_5.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_6] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_6.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_7] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_7.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_8] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_8.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_9] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_9.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_10] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_10.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_11] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_11.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_12] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_12.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_13] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_13.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_14] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_14.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SHIP_EXPLOSION_15] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ship_Explosion_15.bmp", 1);
	
	// Overlays
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_HIT_OVERLAY] = new TextureDisplayOverlay(CONFIGURATION_PATH_TEXTURES "/Player_Hit_Overlay.bmp", 1);
	
	return 0;
}

void uninitialize()
{
	int i;
	
	// Free all tiles
	for (i = 0; i < TEXTURE_IDS_COUNT; i++) delete _pointerTextureManagerTextures[i];
}

Texture *getTextureFromId(TextureId id)
{
	// Make sure the provided ID is valid
	if (id >= TEXTURE_IDS_COUNT) return NULL;
	
	return _pointerTextureManagerTextures[id];
}

}