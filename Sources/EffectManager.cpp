/** @file EffectManager.cpp
 * See EffectManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTextureStaticEntity.hpp>
#include <AudioManager.hpp>
#include <cassert>
#include <EffectManager.hpp>
#include <list>
#include <Log.hpp>
#include <TextureManager.hpp>

namespace EffectManager
{
	/** Gather all data required to generate an effect. */
	typedef struct
	{
		TextureManager::TextureId textureId; //!< The animation texture ID.
		AudioManager::SoundId soundId; //!< The sound to play.
		int availableSounds; //!< How many available sound (their IDs must be contiguous to soundId), a sound will be randomly picked up from the available ones.
	} Effect;

	/** Cache all effect parameters. */
	static Effect effects[EFFECT_IDS_COUNT] =
	{
		// EFFECT_ID_SMALL_ENEMY_EXPLOSION
		{
			TextureManager::TEXTURE_ID_SMALL_ENEMY_EXPLOSION,
			AudioManager::SOUND_ID_SMALL_ENEMY_EXPLOSION,
			1
		},
		// EFFECT_ID_MEDIUM_ENEMY_EXPLOSION
		{
			TextureManager::TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION,
			AudioManager::SOUND_ID_MEDIUM_ENEMY_EXPLOSION,
			1
		},
		// EFFECT_ID_BIG_ENEMY_EXPLOSION
		{
			TextureManager::TEXTURE_ID_BIG_ENEMY_EXPLOSION,
			AudioManager::SOUND_ID_BIG_ENEMY_EXPLOSION,
			1
		},
		// EFFECT_ID_ENEMY_SPAWNER_EXPLOSION
		{
			TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION,
			AudioManager::SOUND_ID_ENEMY_SPAWNER_EXPLOSION,
			1
		},
		// EFFECT_ID_BULLET_EXPLOSION_ENEMY_HIT
		{
			TextureManager::TEXTURE_ID_BULLET_EXPLOSION,
			AudioManager::SOUND_ID_ENEMY_BULLET_IMPACT,
			1
		},
		// EFFECT_ID_BULLET_EXPLOSION_ENEMY_SPAWNER_HIT
		{
			TextureManager::TEXTURE_ID_BULLET_EXPLOSION,
			AudioManager::SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT,
			1
		},
		// EFFECT_ID_BULLET_EXPLOSION_NO_SOUND
		{
			TextureManager::TEXTURE_ID_BULLET_EXPLOSION,
			AudioManager::SOUND_ID_NO_SOUND,
			1
		},
		// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_UP
		{
			TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP,
			AudioManager::SOUND_ID_PLAYER_FIRESHOT_1,
			3
		},
		// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN
		{
			TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN,
			AudioManager::SOUND_ID_PLAYER_FIRESHOT_1,
			3
		},
		// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT
		{
			TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT,
			AudioManager::SOUND_ID_PLAYER_FIRESHOT_1,
			3
		},
		// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT
		{
			TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT,
			AudioManager::SOUND_ID_PLAYER_FIRESHOT_1,
			3
		},
		// EFFECT_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL
		{
			TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL,
			AudioManager::SOUND_ID_PLAYER_FIRESHOT_MORTAR_SHELL,
			1
		},
		// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP
		{
			TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP,
			AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN
		{
			TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN,
			AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT
		{
			TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT,
			AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT
		{
			TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
			AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP
		{
			TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP,
			AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN
		{
			TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN,
			AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT
		{
			TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT,
			AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT
		{
			TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
			AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP
		{
			TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP,
			AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN
		{
			TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN,
			AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT
		{
			TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT,
			AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT
		{
			TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
			AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT,
			1
		},
		// EFFECT_ID_MEDIPACK_TAKEN
		{
			TextureManager::TEXTURE_ID_MEDIPACK_TAKEN,
			AudioManager::SOUND_ID_PLAYER_HEALED,
			1
		},
		// EFFECT_ID_GOLDEN_MEDIPACK_TAKEN
		{
			TextureManager::TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN,
			AudioManager::SOUND_ID_PLAYER_LIFE_INCREASED,
			1
		},
		// EFFECT_ID_AMMUNITION_TAKEN
		{
			TextureManager::TEXTURE_ID_AMMUNITION_TAKEN,
			AudioManager::SOUND_ID_AMMUNITION_TAKEN,
			1
		},
		// EFFECT_ID_MACHINE_GUN_TAKEN
		{
			TextureManager::TEXTURE_ID_MACHINE_GUN_TAKEN,
			AudioManager::SOUND_ID_MACHINE_GUN_TAKEN,
			1
		},
		// EFFECT_ID_BULLETPROOF_VEST_TAKEN
		{
			TextureManager::TEXTURE_ID_BULLETPROOF_VEST_TAKEN,
			AudioManager::SOUND_ID_BULLETPROOF_VEST_TAKEN,
			1
		}
	};

	/** All animated textures. */
	static std::list<AnimatedTextureStaticEntity *> _animatedTexturesList;

	int getEffectTextureWidth(EffectId effectId)
	{
		// Make sure the requested effect is existing
		assert(effectId < EFFECT_IDS_COUNT);
		
		return TextureManager::getTextureFromId(effects[effectId].textureId)->getWidth();
	}

	int getEffectTextureHeight(EffectId effectId)
	{
		// Make sure the requested effect is existing
		assert(effectId < EFFECT_IDS_COUNT);
		
		return TextureManager::getTextureFromId(effects[effectId].textureId)->getHeight();
	}

	void addEffect(int x, int y, EffectId effectId, int soundSourceAngle, int soundSourceDistance)
	{
		// Make sure the requested effect is existing
		assert(effectId < EFFECT_IDS_COUNT);
		
		// Cache effect access
		Effect *pointerEffect = &effects[effectId];
		
		// Generate the graphic effect
		_animatedTexturesList.push_front(new AnimatedTextureStaticEntity(x, y, TextureManager::createAnimatedTextureFromId(pointerEffect->textureId, false)));
		
		// Randomly select a sound if more are available
		AudioManager::SoundId soundId;
		if (pointerEffect->availableSounds > 1)
		{
			int randomIndex = rand() % pointerEffect->availableSounds;
			soundId = static_cast<AudioManager::SoundId>(pointerEffect->soundId + randomIndex);
		}
		else soundId = pointerEffect->soundId; // There is only one sound, select it
		
		// Play audio effect
		AudioManager::playSound(soundId, soundSourceAngle, soundSourceDistance);
	}

	void clearAllEffects()
	{
		std::list<AnimatedTextureStaticEntity *>::iterator animatedTexturesListIterator;
		for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) delete *animatedTexturesListIterator;
		_animatedTexturesList.clear();
	}

	void update()
	{
		std::list<AnimatedTextureStaticEntity *>::iterator animatedTexturesListIterator = _animatedTexturesList.begin();
		AnimatedTextureStaticEntity *pointerAnimatedTextureStaticEntity;
		while (animatedTexturesListIterator != _animatedTexturesList.end())
		{
			pointerAnimatedTextureStaticEntity = *animatedTexturesListIterator;
			
			if (pointerAnimatedTextureStaticEntity->update() != 0)
			{
				// Remove the texture
				delete pointerAnimatedTextureStaticEntity;
				animatedTexturesListIterator = _animatedTexturesList.erase(animatedTexturesListIterator);
			}
			// Animation is not finished, check next one
			else ++animatedTexturesListIterator;
		}
	}

	void render()
	{
		std::list<AnimatedTextureStaticEntity *>::iterator animatedTexturesListIterator;
		for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) (*animatedTexturesListIterator)->render();
	}
}
