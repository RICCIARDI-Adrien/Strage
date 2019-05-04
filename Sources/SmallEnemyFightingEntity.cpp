/** @file SmallEnemyFightingEntity.cpp
 * See SmallEnemyFightingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <EffectManager.hpp>
#include <SmallEnemyFightingEntity.hpp>
#include <TextureManager.hpp>

SmallEnemyFightingEntity::SmallEnemyFightingEntity(int x, int y): EnemyFightingEntity(x, y, TextureManager::TEXTURE_ID_SMALL_ENEMY_FACING_UP, 2, 1, 1000, TextureManager::TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP, EffectManager::EFFECT_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP, EffectManager::EFFECT_ID_SMALL_ENEMY_EXPLOSION) {}

SmallEnemyFightingEntity::~SmallEnemyFightingEntity() {}

BulletMovingEntity *SmallEnemyFightingEntity::_fireBullet(int x, int y)
{
	return new BulletMovingEntity(x, y, TextureManager::TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP, 6, _facingDirection, 1, 0);
}
