/** @file TextureManager.cpp
 * @see TextureManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTexture.hpp>
#include <cassert>
#include <cstdio>
#include <FileManager.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Texture.hpp>
#include <TextureManager.hpp>

namespace TextureManager
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All internal data corresponding to each texture. */
typedef struct
{
	const char *pointerStringFileName; //!< The file the texture is loaded from.
	int imagesCount; //!< How many images are contained in the provided file (in case of an animated texture). Set to 1 if the texture is not animated.
	int framesPerImageCount; //!< Used only for animated textures. How many game frame to wait before displaying the next image.
	SDL_Texture *pointerSDLTexture; //!< The SDL texture that can be rendered.
	Texture *pointerTexture; //!< The game texture representation associated with this image (it can be still or animated).
} TextureInformation;
	
//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All game available textures. */
static TextureInformation _texturesInformations[] =
{
	// Scene floors
	// TEXTURE_ID_SCENE_FLOOR_0
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Clay_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_1
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Dirt_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_2
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Dirt_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_3
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Grass_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_4
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Grass_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_5
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Gravel_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_6
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Gravel_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_7
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Grid_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_8
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Level_Exit.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_9
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Plate_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_10
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Plate_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_11
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Sand_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_12
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Snow_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_13
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Stone_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_14
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Stone_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_15
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Stone_2.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_FLOOR_16
	{
		CONFIGURATION_PATH_TEXTURES "/Floor_Stone_3.bmp",
		1,
		0,
		NULL,
		NULL
	},
	
	// Scene walls
	// TEXTURE_ID_SCENE_WALL_0
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Stone_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_WALL_1
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Stone_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_WALL_2
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Stone_2.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_WALL_3
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Stone_3.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_WALL_4
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Stone_4.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_WALL_5
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Bricks_0.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SCENE_WALL_6
	{
		CONFIGURATION_PATH_TEXTURES "/Wall_Bricks_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	
	// Static objects
	// TEXTURE_ID_MEDIPACK
	{
		CONFIGURATION_PATH_TEXTURES "/Medipack.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_GOLDEN_MEDIPACK
	{
		CONFIGURATION_PATH_TEXTURES "/Golden_Medipack.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_AMMUNITION
	{
		CONFIGURATION_PATH_TEXTURES "/Ammunition.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_ENEMY_SPAWNER
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_1
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_1.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_2
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_2.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_3
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_3.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_4
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_4.bmp",
		1,
		0,
		NULL,
		NULL
	},
	
	// Moving objects
	// TEXTURE_ID_PLAYER_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Up.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Down.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Left.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Right.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Up.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Down.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Left.bmp",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Right.bmp",
		1,
		0,
		NULL,
		NULL
	},
	
	// Overlays
	// TEXTURE_ID_PLAYER_HIT_OVERLAY
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Hit_Overlay.bmp",
		1,
		0,
		NULL,
		NULL
	},
	
	// Graphic user interface
	// TEXTURE_ID_HEAD_UP_DISPLAY_BACKGROUND
	{
		CONFIGURATION_PATH_TEXTURES "/Head_Up_Display_Background.png",
		1,
		0,
		NULL,
		NULL
	},
	// TEXTURE_ID_MENU_BACKGROUND
	{
		CONFIGURATION_PATH_TEXTURES "/Menu_Background.bmp",
		1,
		0,
		NULL,
		NULL
	},
	
	// Animated textures
	// TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion.bmp",
		16,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_EXPLOSION
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Explosion.bmp",
		16,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Explosion.bmp",
		16,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_EXPLOSION
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Explosion.bmp",
		16,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Up.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Down.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Left.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Right.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Up.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Down.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Left.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Right.bmp",
		1,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Up.bmp",
		1,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Down.bmp",
		1,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Left.bmp",
		1,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Right.bmp",
		1,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Up.bmp",
		1,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Down.bmp",
		1,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Left.bmp",
		1,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT
	{
		CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Right.bmp",
		1,
		8,
		NULL,
		NULL
	},
	// TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL
	{
		CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Mortar_Shell.bmp",
		6,
		4,
		NULL,
		NULL
	},
	// TEXTURE_ID_MEDIPACK_TAKEN
	{
		CONFIGURATION_PATH_TEXTURES "/Medipack_Taken.bmp",
		7,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN
	{
		CONFIGURATION_PATH_TEXTURES "/Golden_Medipack_Taken.bmp",
		7,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_AMMUNITION_TAKEN
	{
		CONFIGURATION_PATH_TEXTURES "/Ammunition_Taken.bmp",
		3,
		6,
		NULL,
		NULL
	},
	// TEXTURE_ID_ENEMY_SPAWNER_TELEPORTATION_EFFECT
	{
		CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Teleportation_Effect.bmp",
		8,
		16,
		NULL,
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	int i;
	SDL_Surface *pointerSurface;
	unsigned int transparentColor;
	
	// Load all files
	for (i = 0; i < TEXTURE_IDS_COUNT; i++)
	{
		// Try to load bitmap
		pointerSurface = IMG_Load(_texturesInformations[i].pointerStringFileName);
		if (pointerSurface == NULL)
		{
			LOG_ERROR("Failed to load texture '%s' (%s).", _texturesInformations[i].pointerStringFileName, SDL_GetError());
			return -1;
		}
		
		// Set the transparent color as bright green (lime)
		transparentColor = SDL_MapRGB(pointerSurface->format, 0x00, 0xFF, 0x00);
		if (SDL_SetColorKey(pointerSurface, SDL_TRUE, transparentColor) != 0)
		{
			LOG_ERROR("Failed to set the surface transparent color (%s).", SDL_GetError());
			return -1;
		}
		
		// Convert the surface to a texture
		_texturesInformations[i].pointerSDLTexture = SDL_CreateTextureFromSurface(Renderer::pointerRenderer, pointerSurface);
		SDL_FreeSurface(pointerSurface);
		if (_texturesInformations[i].pointerSDLTexture == NULL)
		{
			LOG_ERROR("Failed to convert the surface to a texture (%s).", SDL_GetError());
			return -1;
		}
		
		// Create the associated game texture (TODO even for animated textures ?)
		_texturesInformations[i].pointerTexture = new Texture(_texturesInformations[i].pointerSDLTexture);
	}
	
	return 0;
}

void uninitialize()
{
	int i;
	
	// Free all textures
	for (i = 0; i < TEXTURE_IDS_COUNT; i++)
	{
		delete _texturesInformations[i].pointerTexture;
		SDL_DestroyTexture(_texturesInformations[i].pointerSDLTexture);
	}
}

Texture *getTextureFromId(TextureId id)
{
	// Make sure the provided ID is valid
	assert(id < TEXTURE_IDS_COUNT);
	
	return _texturesInformations[id].pointerTexture;
}

AnimatedTexture *createAnimatedTextureFromId(TextureId id, bool isAnimationLooping)
{
	// Make sure the provided ID is valid
	assert(id < TEXTURE_IDS_COUNT);
	
	return new AnimatedTexture(_texturesInformations[id].pointerSDLTexture, _texturesInformations[id].imagesCount, _texturesInformations[id].framesPerImageCount, isAnimationLooping);
}

}
