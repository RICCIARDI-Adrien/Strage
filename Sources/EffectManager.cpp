/** @file EffectManager.cpp
 * @see EffectManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <EffectManager.hpp>
#include <EntityAnimatedTexture.hpp>
#include <Log.hpp>
#include <TextureManager.hpp>

namespace EffectManager
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Gather all data required to generate an effect. */
typedef struct
{
	TextureManager::TextureId initialTextureId; //!< The texture the animation starts with.
	TextureManager::TextureId finalTextureId; //!< The texture the animation ends with.
	int framesPerTexture; //!< How many game engine frames to wait before displaying the next texture.
	AudioManager::SoundId soundId; //!< The sound to play.
} Effect;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Cache all effect parameters. */
static Effect effects[EFFECT_IDS_COUNT] =
{
	// EFFECT_ID_SMALL_ENEMY_EXPLOSION
	{
		TextureManager::TEXTURE_ID_SMALL_ENEMY_EXPLOSION_0,
		TextureManager::TEXTURE_ID_SMALL_ENEMY_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_ENEMY_EXPLOSION
	},
	// EFFECT_ID_MEDIUM_ENEMY_EXPLOSION
	{
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_0,
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_ENEMY_EXPLOSION
	},
	// EFFECT_ID_BIG_ENEMY_EXPLOSION
	{
		TextureManager::TEXTURE_ID_BIG_ENEMY_EXPLOSION_0,
		TextureManager::TEXTURE_ID_BIG_ENEMY_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_ENEMY_EXPLOSION
	},
	// EFFECT_ID_ENEMY_SPAWNER_EXPLOSION
	{
		TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_0,
		TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_ENEMY_SPAWNER_EXPLOSION
	},
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_UP
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_DOWN
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_LEFT
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_RIGHT
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_UP
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_DOWN
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_LEFT
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_RIGHT
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_UP
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_DOWN
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_LEFT
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_RIGHT
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_UP
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_DOWN
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_LEFT
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_RIGHT
};

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
EntityAnimatedTexture *generateEffect(int x, int y, EffectId effectId)
{
	Effect *pointerEffect;
	
	// Make sure the requested effect is existing
	if (effectId >= EFFECT_IDS_COUNT)
	{
		LOG_ERROR("Non-existing effect requested (effect ID : %d)", effectId);
		return NULL;
	}
	
	// Cache effect access
	pointerEffect = &effects[effectId];
	
	// Play audio effect
	AudioManager::playSound(pointerEffect->soundId);
	
	// Generate the graphic effect
	return new EntityAnimatedTexture(x, y, pointerEffect->initialTextureId, pointerEffect->finalTextureId, pointerEffect->framesPerTexture);
}

}
