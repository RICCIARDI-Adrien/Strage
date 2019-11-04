/** @file TextureManager.cpp
 * See TextureManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTexture.hpp>
#include <cassert>
#include <Configuration.hpp>
#include <cstdio>
#include <FileManager.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#ifdef CONFIGURATION_BUILD_FOR_MACOS
	#include <SDL2_image/SDL_image.h>
#else
	#include <SDL2/SDL_image.h>
#endif
#include <Texture.hpp>
#include <TextureManager.hpp>

namespace TextureManager
{
	/** All internal data corresponding to each texture. */
	typedef struct
	{
		const char *pointerStringFileName; //!< The file the texture is loaded from.
		int imagesCount; //!< How many images are contained in the provided file (in case of an animated texture). Set to 1 if the texture is not animated.
		int framesPerImageCount; //!< Used only for animated textures. How many game frame to wait before displaying the next image.
		SDL_Texture *pointerSDLTexture; //!< The SDL texture that can be rendered.
		Texture *pointerTexture; //!< The game texture representation associated with this image (it can be still or animated).
	} TextureInformation;

	/** All game available textures. */
	static TextureInformation _texturesInformations[] =
	{
		// Scene floors
		// TEXTURE_ID_SCENE_FLOOR_0
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Clay_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_1
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Dirt_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_2
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Dirt_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_3
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Grass_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_4
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Grass_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_5
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Gravel_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_6
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Gravel_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_7
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Grid_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_8
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Level_Exit.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_9
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Plate_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_10
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Metal_Plate_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_11
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Sand_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_12
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Snow_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_13
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Stone_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_14
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Stone_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_15
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Stone_2.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_FLOOR_16
		{
			CONFIGURATION_PATH_TEXTURES "/Floor_Stone_3.png",
			1,
			0,
			NULL,
			NULL
		},
		
		// Scene walls
		// TEXTURE_ID_SCENE_WALL_0
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Stone_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_WALL_1
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Stone_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_WALL_2
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Stone_2.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_WALL_3
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Stone_3.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_WALL_4
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Stone_4.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_WALL_5
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Bricks_0.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SCENE_WALL_6
		{
			CONFIGURATION_PATH_TEXTURES "/Wall_Bricks_1.png",
			1,
			0,
			NULL,
			NULL
		},
		
		// Static objects
		// TEXTURE_ID_MEDIPACK
		{
			CONFIGURATION_PATH_TEXTURES "/Medipack.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_GOLDEN_MEDIPACK
		{
			CONFIGURATION_PATH_TEXTURES "/Golden_Medipack.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_AMMUNITION
		{
			CONFIGURATION_PATH_TEXTURES "/Ammunition.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MACHINE_GUN
		{
			CONFIGURATION_PATH_TEXTURES "/Machine_Gun.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BULLETPROOF_VEST
		{
			CONFIGURATION_PATH_TEXTURES "/Bulletproof_Vest.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_ENEMY_SPAWNER
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_1
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_1.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_2
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_2.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_3
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_3.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_4
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Damaged_4.png",
			1,
			0,
			NULL,
			NULL
		},
		
		// Moving objects
		// TEXTURE_ID_PLAYER_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bulletproof_Vest_Bonus_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bulletproof_Vest_Bonus_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bulletproof_Vest_Bonus_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bulletproof_Vest_Bonus_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Machine_Gun_Bonus_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Machine_Gun_Bonus_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Machine_Gun_Bonus_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Machine_Gun_Bonus_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_1_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Damaged_2_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_1_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_2_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_3_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Damaged_4_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Bullet_Mortar_Shell_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Up.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Down.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Left.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Bullet_Right.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Bullet_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Up.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Down.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Left.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_BULLET_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Bullet_Right.png",
			1,
			0,
			NULL,
			NULL
		},
		
		// Overlays
		// TEXTURE_ID_PLAYER_HIT_OVERLAY
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Hit_Overlay.png",
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
			CONFIGURATION_PATH_TEXTURES "/Menu_Background.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_UP_TURNED_OFF
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Up_Turned_Off.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_DOWN_TURNED_OFF
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Down_Turned_Off.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_LEFT_TURNED_OFF
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Left_Turned_Off.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_RIGHT_TURNED_OFF
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Right_Turned_Off.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_UP_LIGHTED
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Up_Lighted.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_DOWN_LIGHTED
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Down_Lighted.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_LEFT_LIGHTED
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Left_Lighted.png",
			1,
			0,
			NULL,
			NULL
		},
		// TEXTURE_ID_COMPASS_ARROW_RIGHT_LIGHTED
		{
			CONFIGURATION_PATH_TEXTURES "/Compass_Arrow_Right_Lighted.png",
			1,
			0,
			NULL,
			NULL
		},
		
		// Animated textures
		// TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Explosion.png",
			32,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_EXPLOSION
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Explosion.png",
			32,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Explosion.png",
			32,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_EXPLOSION
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Explosion.png",
			32,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_BULLET_EXPLOSION
		{
			CONFIGURATION_PATH_TEXTURES "/Bullet_Explosion.png",
			3,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Up.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Down.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Left.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Right.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Up.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Down.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Left.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Small_Enemy_Muzzle_Flash_Right.png",
			1,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Up.png",
			1,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Down.png",
			1,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Left.png",
			1,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Medium_Enemy_Muzzle_Flash_Right.png",
			1,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Up.png",
			1,
			8,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Down.png",
			1,
			8,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Left.png",
			1,
			8,
			NULL,
			NULL
		},
		// TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT
		{
			CONFIGURATION_PATH_TEXTURES "/Big_Enemy_Muzzle_Flash_Right.png",
			1,
			8,
			NULL,
			NULL
		},
		// TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL
		{
			CONFIGURATION_PATH_TEXTURES "/Player_Muzzle_Flash_Mortar_Shell.png",
			6,
			4,
			NULL,
			NULL
		},
		// TEXTURE_ID_MEDIPACK_TAKEN
		{
			CONFIGURATION_PATH_TEXTURES "/Medipack_Taken.png",
			7,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN
		{
			CONFIGURATION_PATH_TEXTURES "/Golden_Medipack_Taken.png",
			7,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_AMMUNITION_TAKEN
		{
			CONFIGURATION_PATH_TEXTURES "/Ammunition_Taken.png",
			3,
			6,
			NULL,
			NULL
		},
		// TEXTURE_ID_ENEMY_SPAWNER_TELEPORTATION_EFFECT
		{
			CONFIGURATION_PATH_TEXTURES "/Enemy_Spawner_Teleportation_Effect.png",
			8,
			16,
			NULL,
			NULL
		},
		// TEXTURE_ID_MACHINE_GUN_TAKEN
		{
			CONFIGURATION_PATH_TEXTURES "/Machine_Gun_Taken.png",
			8,
			8,
			NULL,
			NULL
		},
		// TEXTURE_ID_BULLETPROOF_VEST_TAKEN
		{
			CONFIGURATION_PATH_TEXTURES "/Bulletproof_Vest_Taken.png",
			8,
			8,
			NULL,
			NULL
		}
	};

	int initialize()
	{
		// Load all files
		SDL_Surface *pointerSurface;
		for (int i = 0; i < TEXTURE_IDS_COUNT; i++)
		{
			// Try to load bitmap
			pointerSurface = IMG_Load(_texturesInformations[i].pointerStringFileName);
			if (pointerSurface == NULL)
			{
				LOG_ERROR("Failed to load texture '%s' (%s).", _texturesInformations[i].pointerStringFileName, SDL_GetError());
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
			
			LOG_DEBUG("Loaded texture. File name : %s, texture ID : %d, SDL texture pointer : %p.", _texturesInformations[i].pointerStringFileName, i, _texturesInformations[i].pointerSDLTexture);
		}
		LOG_DEBUG("Loaded %d textures.", TEXTURE_IDS_COUNT);
		
		return 0;
	}

	void uninitialize()
	{
		// Free all textures
		for (int i = 0; i < TEXTURE_IDS_COUNT; i++)
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
