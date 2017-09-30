/** @file EffectManager.hpp
 * Associate animated textures with sounds to generate a "special" effect on the map, like an explosion or a muzzle flash.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_EFFECT_MANAGER_HPP
#define HPP_EFFECT_MANAGER_HPP

#include <StaticEntityAnimatedTexture.hpp>

namespace EffectManager
{

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All available effects. */
typedef enum
{
	EFFECT_ID_SMALL_ENEMY_EXPLOSION,
	EFFECT_ID_MEDIUM_ENEMY_EXPLOSION,
	EFFECT_ID_BIG_ENEMY_EXPLOSION,
	EFFECT_ID_ENEMY_SPAWNER_EXPLOSION,
	EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_UP,
	EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN,
	EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT,
	EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT,
	EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP,
	EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN,
	EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT,
	EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
	EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP,
	EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN,
	EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT,
	EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
	EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP,
	EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN,
	EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT,
	EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
	EFFECT_IDS_COUNT
} EffectId;

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** Generate the requested effect at the map specified coordinates.
 * @param x Effect horizontal coordinate on the map.
 * @param y Effect vertical coordinate on the map.
 * @param effectId Specify the effect to generate. It's the caller responsibility to free the generated effect.
 * @return NULL if the requested effect does not exist,
 * @return The animated texture corresponding to the effect visual part. Play the effect sound in the same time.
 */
StaticEntityAnimatedTexture *generateEffect(int x, int y, EffectId effectId);

/** Get the effect texture width (it is the same for all effect animated frames).
 * @param effectId The effect to get texture width.
 * @return -1 if the provided effect does not exist,
 * @return The effect textures width.
 */
int getEffectTextureWidth(EffectId effectId);

/** Get the effect texture height (it is the same for all effect animated frames).
 * @param effectId The effect to get texture height.
 * @return -1 if the provided effect does not exist,
 * @return The effect textures height.
 */
int getEffectTextureHeight(EffectId effectId);
	
}

#endif
