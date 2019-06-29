/** @file EffectManager.hpp
 * Associate animated textures with sounds to generate a "special" effect on the map, like an explosion or a muzzle flash.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_EFFECT_MANAGER_HPP
#define HPP_EFFECT_MANAGER_HPP

namespace EffectManager
{
	/** All available effects. */
	typedef enum
	{
		EFFECT_ID_SMALL_ENEMY_EXPLOSION,
		EFFECT_ID_MEDIUM_ENEMY_EXPLOSION,
		EFFECT_ID_BIG_ENEMY_EXPLOSION,
		EFFECT_ID_ENEMY_SPAWNER_EXPLOSION,
		EFFECT_ID_BULLET_EXPLOSION_ENEMY_HIT,
		EFFECT_ID_BULLET_EXPLOSION_ENEMY_SPAWNER_HIT,
		EFFECT_ID_BULLET_EXPLOSION_NO_SOUND,
		EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_UP,
		EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN,
		EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT,
		EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT,
		EFFECT_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL,
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
		EFFECT_ID_MEDIPACK_TAKEN,
		EFFECT_ID_GOLDEN_MEDIPACK_TAKEN,
		EFFECT_ID_AMMUNITION_TAKEN,
		EFFECT_ID_MACHINE_GUN_TAKEN,
		EFFECT_ID_BULLETPROOF_VEST_TAKEN,
		EFFECT_IDS_COUNT
	} EffectId;

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

	/** Add a new effect to display on the scene.
	* @param x Effect horizontal coordinate on the map.
	* @param y Effect vertical coordinate on the map.
	* @param effectId Specify the effect to generate.
	*/
	void addEffect(int x, int y, EffectId effectId);

	/** Delete all currently playing effects. */
	void clearAllEffects();

	/** Update all currently playing effects state.
	* @note Must be called on the game update step.
	*/
	void update();

	/** Display all currently playing effects.
	* @note Must be called on the game rendering step.
	*/
	void render();
}

#endif
