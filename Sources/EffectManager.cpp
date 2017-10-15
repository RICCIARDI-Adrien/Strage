/** @file EffectManager.cpp
 * @see EffectManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <cassert>
#include <EffectManager.hpp>
#include <list>
#include <Log.hpp>
#include <StaticEntityAnimatedTexture.hpp>
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
		AudioManager::SOUND_ID_SMALL_ENEMY_EXPLOSION
	},
	// EFFECT_ID_MEDIUM_ENEMY_EXPLOSION
	{
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_0,
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_MEDIUM_ENEMY_EXPLOSION
	},
	// EFFECT_ID_BIG_ENEMY_EXPLOSION
	{
		TextureManager::TEXTURE_ID_BIG_ENEMY_EXPLOSION_0,
		TextureManager::TEXTURE_ID_BIG_ENEMY_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_BIG_ENEMY_EXPLOSION
	},
	// EFFECT_ID_ENEMY_SPAWNER_EXPLOSION
	{
		TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_0,
		TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_15,
		8,
		AudioManager::SOUND_ID_ENEMY_SPAWNER_EXPLOSION
	},
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_UP
	{
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP,
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP,
		4,
		AudioManager::SOUND_ID_PLAYER_FIRESHOT
	},
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN
	{
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN,
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN,
		4,
		AudioManager::SOUND_ID_PLAYER_FIRESHOT
	},
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT
	{
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT,
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT,
		4,
		AudioManager::SOUND_ID_PLAYER_FIRESHOT
	},
	// EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT
	{
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT,
		TextureManager::TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT,
		4,
		AudioManager::SOUND_ID_PLAYER_FIRESHOT
	},
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP
	{
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP,
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP,
		4,
		AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT
	},
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN
	{
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		4,
		AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT
	},
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT
	{
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		4,
		AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT
	},
	// EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT
	{
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		TextureManager::TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		4,
		AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT
	},
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP
	{
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP,
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP,
		6,
		AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT
	},
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN
	{
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		6,
		AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT
	},
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT
	{
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		6,
		AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT
	},
	// EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT
	{
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		TextureManager::TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		6,
		AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT
	},
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP
	{
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP,
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP,
		8,
		AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT
	},
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN
	{
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		8,
		AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT
	},
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT
	{
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		8,
		AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT
	},
	// EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT
	{
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		TextureManager::TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		8,
		AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT
	},
	// EFFECT_ID_MEDIPACK_TAKEN
	{
		TextureManager::TEXTURE_ID_MEDIPACK_TAKEN_0,
		TextureManager::TEXTURE_ID_MEDIPACK_TAKEN_6,
		6,
		AudioManager::SOUND_ID_PLAYER_HEALED
	},
	// EFFECT_ID_GOLDEN_MEDIPACK_TAKEN
	{
		TextureManager::TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_0,
		TextureManager::TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN_6,
		6,
		AudioManager::SOUND_ID_PLAYER_LIFE_INCREASED
	},
	// EFFECT_ID_AMMUNITION_TAKEN
	{
		TextureManager::TEXTURE_ID_AMMUNITION_TAKEN_0,
		TextureManager::TEXTURE_ID_AMMUNITION_TAKEN_2,
		6,
		AudioManager::SOUND_ID_AMMUNITION_TAKEN
	}
};

/** All animated textures. */
static std::list<StaticEntityAnimatedTexture *> _animatedTexturesList;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int getEffectTextureWidth(EffectId effectId)
{
	// Make sure the requested effect is existing
	assert(effectId < EFFECT_IDS_COUNT);
	
	return TextureManager::getTextureFromId(effects[effectId].initialTextureId)->getWidth();
}

int getEffectTextureHeight(EffectId effectId)
{
	// Make sure the requested effect is existing
	assert(effectId < EFFECT_IDS_COUNT);
	
	return TextureManager::getTextureFromId(effects[effectId].initialTextureId)->getHeight();
}

void addEffect(int x, int y, EffectId effectId)
{
	Effect *pointerEffect;
	
	// Make sure the requested effect is existing
	assert(effectId < EFFECT_IDS_COUNT);
	
	// Cache effect access
	pointerEffect = &effects[effectId];
	
	// Generate the graphic effect
	_animatedTexturesList.push_front(new StaticEntityAnimatedTexture(x, y, pointerEffect->initialTextureId, pointerEffect->finalTextureId, pointerEffect->framesPerTexture));
	
	// Play audio effect
	AudioManager::playSound(pointerEffect->soundId);
}

void clearAllEffects()
{
	std::list<StaticEntityAnimatedTexture *>::iterator animatedTexturesListIterator;
	
	for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) delete *animatedTexturesListIterator;
	_animatedTexturesList.clear();
}

void update()
{
	std::list<StaticEntityAnimatedTexture *>::iterator animatedTexturesListIterator = _animatedTexturesList.begin();
	StaticEntityAnimatedTexture *pointerAnimatedTexture;
	
	while (animatedTexturesListIterator != _animatedTexturesList.end())
	{
		pointerAnimatedTexture = *animatedTexturesListIterator;
		
		if (pointerAnimatedTexture->update() != 0)
		{
			// Remove the texture
			delete pointerAnimatedTexture;
			animatedTexturesListIterator = _animatedTexturesList.erase(animatedTexturesListIterator);
		}
		// Animation is not finished, check next one
		else ++animatedTexturesListIterator;
	}
}

void render()
{
	std::list<StaticEntityAnimatedTexture *>::iterator animatedTexturesListIterator;
	
 	for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) (*animatedTexturesListIterator)->render();
}

}
