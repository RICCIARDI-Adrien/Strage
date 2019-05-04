/** @file BigEnemyFightingEntity.cpp
 * See BigEnemyFightingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <BigEnemyFightingEntity.hpp>
#include <EffectManager.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

BigEnemyFightingEntity::BigEnemyFightingEntity(int x, int y): EnemyFightingEntity(x, y, TextureManager::TEXTURE_ID_BIG_ENEMY_FACING_UP, 1, 10, 10000, TextureManager::TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP, EffectManager::EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP, EffectManager::EFFECT_ID_BIG_ENEMY_EXPLOSION)
{
	// Make big enemies wait 3 seconds before shooting (to avoid them shooting instantly when spawned)
	unsigned int currentTime = SDL_GetTicks();
	if (currentTime > 7000) _lastShotTime = currentTime - 7000;
}

BigEnemyFightingEntity::~BigEnemyFightingEntity() {}

int BigEnemyFightingEntity::update()
{
	// Update textures to show enemy damage state
	if (_lifePointsAmount == 8)
	{
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_RIGHT);
	}
	else if (_lifePointsAmount == 6)
	{
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_RIGHT);
	}
	else if (_lifePointsAmount == 4)
	{
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_RIGHT);
	}
	else if (_lifePointsAmount == 2)
	{
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_RIGHT);
	}
	
	return EnemyFightingEntity::update();
}

BulletMovingEntity *BigEnemyFightingEntity::_fireBullet(int x, int y)
{
	return new BulletMovingEntity(x, y, TextureManager::TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP, 4, _facingDirection, 20, 0);
}
