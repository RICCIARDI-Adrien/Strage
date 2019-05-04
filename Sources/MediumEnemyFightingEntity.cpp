/** @file MediumEnemyFightingEntity.cpp
 * See MediumEnemyFightingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <EffectManager.hpp>
#include <MediumEnemyFightingEntity.hpp>
#include <TextureManager.hpp>

MediumEnemyFightingEntity::MediumEnemyFightingEntity(int x, int y): EnemyFightingEntity(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_FACING_UP, 2, 3, 2000, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP, EffectManager::EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP, EffectManager::EFFECT_ID_MEDIUM_ENEMY_EXPLOSION) {}

MediumEnemyFightingEntity::~MediumEnemyFightingEntity() {}

int MediumEnemyFightingEntity::update()
{
	// Update textures to show enemy damage state
	if (_lifePointsAmount == 2)
	{
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_RIGHT);
	}
	else if (_lifePointsAmount == 1)
	{
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_RIGHT);
	}
	
	return EnemyFightingEntity::update();
}

BulletMovingEntity *MediumEnemyFightingEntity::_fireBullet(int x, int y)
{
	return new BulletMovingEntity(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP, 5, _facingDirection, 3, 0);
}
