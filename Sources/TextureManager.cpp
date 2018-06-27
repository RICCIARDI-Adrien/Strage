/** @file TextureManager.cpp
 * @see TextureManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <cstdio>
#include <FileManager.hpp>
#include <Log.hpp>
#include <Texture.hpp>
#include <TextureDisplayOverlay.hpp>
#include <TextureManager.hpp>

namespace TextureManager
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All internal data corresponding to each texture. */
typedef struct
{
	char *pointerStringFileName; //!< The file the texture is loaded from.
	SDL_Texture *pointerSDLTexture; //!< The SDL texture that can be rendered.
	Texture *pointerTexture; //!< The game texture representation associated with this image (it can be still or animated).
} TextureInformation;
	
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
	// Scene floors
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_0] = new Texture(FileManager::getFilePath("Textures/Floor_Clay_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_1] = new Texture(FileManager::getFilePath("Textures/Floor_Dirt_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_2] = new Texture(FileManager::getFilePath("Textures/Floor_Dirt_1.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_3] = new Texture(FileManager::getFilePath("Textures/Floor_Grass_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_4] = new Texture(FileManager::getFilePath("Textures/Floor_Grass_1.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_5] = new Texture(FileManager::getFilePath("Textures/Floor_Gravel_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_6] = new Texture(FileManager::getFilePath("Textures/Floor_Gravel_1.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_7] = new Texture(FileManager::getFilePath("Textures/Floor_Metal_Grid_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_8] = new Texture(FileManager::getFilePath("Textures/Floor_Level_Exit.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_9] = new Texture(FileManager::getFilePath("Textures/Floor_Metal_Plate_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_10] = new Texture(FileManager::getFilePath("Textures/Floor_Metal_Plate_1.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_11] = new Texture(FileManager::getFilePath("Textures/Floor_Sand_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_12] = new Texture(FileManager::getFilePath("Textures/Floor_Snow_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_13] = new Texture(FileManager::getFilePath("Textures/Floor_Stone_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_14] = new Texture(FileManager::getFilePath("Textures/Floor_Stone_1.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_15] = new Texture(FileManager::getFilePath("Textures/Floor_Stone_2.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_FLOOR_16] = new Texture(FileManager::getFilePath("Textures/Floor_Stone_3.bmp"), 0);
	
	// Scene walls
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_0] = new Texture(FileManager::getFilePath("Textures/Wall_Stone_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_1] = new Texture(FileManager::getFilePath("Textures/Wall_Stone_1.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_2] = new Texture(FileManager::getFilePath("Textures/Wall_Stone_2.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_3] = new Texture(FileManager::getFilePath("Textures/Wall_Stone_3.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_4] = new Texture(FileManager::getFilePath("Textures/Wall_Stone_4.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_6] = new Texture(FileManager::getFilePath("Textures/Wall_Bricks_0.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SCENE_WALL_7] = new Texture(FileManager::getFilePath("Textures/Wall_Bricks_1.bmp"), 0);
	
	// Static objects
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK] = new Texture(FileManager::getFilePath("Textures/Medipack.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_AMMUNITION] = new Texture(FileManager::getFilePath("Textures/Ammunition.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner.bmp"), 1);
	
	// Moving objects
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Player_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Player_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Player_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Player_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Up.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Down.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Left.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Right.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Mortar_Shell_Up.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Mortar_Shell_Down.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Mortar_Shell_Left.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Player_Bullet_Mortar_Shell_Right.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Bullet_Up.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Bullet_Down.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Bullet_Left.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Bullet_Right.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Bullet_Up.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Bullet_Down.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Bullet_Left.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Bullet_Right.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Bullet_Up.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Bullet_Down.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Bullet_Left.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_BULLET_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Bullet_Right.bmp"), 0);
	
	// Animated textures
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_0] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_1] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_2] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_3] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_4] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_5] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_6] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_6.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_7] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_7.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_8] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_8.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_9] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_9.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_10] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_10.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_11] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_11.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_12] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_12.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_13] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_13.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_14] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_14.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_15] = new Texture(FileManager::getFilePath("Textures/Enemy_Spawner_Explosion_15.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_0] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_1] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_2] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_3] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_4] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_5] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_6] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_6.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_7] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_7.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_8] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_8.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_9] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_9.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_10] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_10.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_11] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_11.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_12] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_12.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_13] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_13.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_14] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_14.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_EXPLOSION_15] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Explosion_15.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_0] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_1] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_2] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_3] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_4] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_5] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_6] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_6.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_7] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_7.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_8] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_8.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_9] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_9.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_10] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_10.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_11] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_11.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_12] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_12.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_13] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_13.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_14] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_14.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_15] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Explosion_15.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_0] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_1] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_2] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_3] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_4] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_5] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_6] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_6.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_7] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_7.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_8] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_8.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_9] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_9.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_10] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_10.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_11] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_11.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_12] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_12.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_13] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_13.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_14] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_14.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_15] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_15.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_EXPLOSION_test] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Explosion_test.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_0] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Mortar_Shell_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_1] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Mortar_Shell_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_2] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Mortar_Shell_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_3] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Mortar_Shell_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_4] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Mortar_Shell_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_5] = new Texture(FileManager::getFilePath("Textures/Player_Muzzle_Flash_Mortar_Shell_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Muzzle_Flash_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Muzzle_Flash_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Muzzle_Flash_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Small_Enemy_Muzzle_Flash_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Muzzle_Flash_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Muzzle_Flash_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Muzzle_Flash_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Medium_Enemy_Muzzle_Flash_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Muzzle_Flash_Up.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Muzzle_Flash_Down.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Muzzle_Flash_Left.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT] = new Texture(FileManager::getFilePath("Textures/Big_Enemy_Muzzle_Flash_Right.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_0] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_1] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_2] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_3] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_4] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_5] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_MEDIPACK_TAKEN_6] = new Texture(FileManager::getFilePath("Textures/Medipack_Taken_6.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_0] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_1] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_2] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_2.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_3] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_3.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_4] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_4.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_5] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_5.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_6] = new Texture(FileManager::getFilePath("Textures/Golden_Medipack_Taken_6.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_AMMUNITION_TAKEN_0] = new Texture(FileManager::getFilePath("Textures/Ammunition_Taken_0.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_AMMUNITION_TAKEN_1] = new Texture(FileManager::getFilePath("Textures/Ammunition_Taken_1.bmp"), 1);
	_pointerTextureManagerTextures[TEXTURE_ID_AMMUNITION_TAKEN_2] = new Texture(FileManager::getFilePath("Textures/Ammunition_Taken_2.bmp"), 1);
	
	// Overlays
	_pointerTextureManagerTextures[TEXTURE_ID_PLAYER_HIT_OVERLAY] = new TextureDisplayOverlay(FileManager::getFilePath("Textures/Player_Hit_Overlay.bmp"), 1);
	
	// Graphic user interface
	_pointerTextureManagerTextures[TEXTURE_ID_GRAPHIC_USER_INTERFACE_BACKGROUND] = new Texture(FileManager::getFilePath("Textures/Graphic_User_Interface_Background.bmp"), 0);
	_pointerTextureManagerTextures[TEXTURE_ID_MENU_BACKGROUND] = new Texture(FileManager::getFilePath("Textures/Menu_Background.bmp"), 0);
	
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
	assert(id < TEXTURE_IDS_COUNT);
	
	return _pointerTextureManagerTextures[id];
}

}
