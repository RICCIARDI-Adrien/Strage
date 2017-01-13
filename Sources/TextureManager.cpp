/** @file TextureManager.cpp
 * @see TextureManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstdio>
#include <Log.hpp>
#include <Texture.hpp>
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
	// Scene tiles
	_pointerTextureManagerTextures[TEXTURE_ID_RIVER_SAND] = new Texture(CONFIGURATION_PATH_TEXTURES "/River_Sand.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_GREEN_GRASS] = new Texture(CONFIGURATION_PATH_TEXTURES "/Grass.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_WALL_STONE_1] = new Texture(CONFIGURATION_PATH_TEXTURES "/Wall_Stone_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_DIRT_1] = new Texture(CONFIGURATION_PATH_TEXTURES "/Dirt_1.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_DIRT_2] = new Texture(CONFIGURATION_PATH_TEXTURES "/Dirt_2.bmp", 0);
	
	// Static objects
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK] = new Texture(CONFIGURATION_PATH_TEXTURES "/Medipack.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_AMMUNITION] = new Texture(CONFIGURATION_PATH_TEXTURES "/Ammunition.bmp", 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner.bmp", 0);
	
	// Movable objects
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER] = new Texture(CONFIGURATION_PATH_TEXTURES "/Player.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY] = new Texture(CONFIGURATION_PATH_TEXTURES "/Enemy.bmp", 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BULLET] = new Texture(CONFIGURATION_PATH_TEXTURES "/Bullet.bmp", 0);
	
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