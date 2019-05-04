/** @file BulletMovingEntity.cpp
 * See BulletMovingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <BulletMovingEntity.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

BulletMovingEntity::BulletMovingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, Direction facingDirection, int damageAmount, bool isEnemySpawnerDamageable): MovingEntity(x, y, textureId, movingPixelsAmount)
{
	_range = 2 * Renderer::displayWidth;
	_facingDirection = facingDirection;
	_movedDistance = 0;
	_damageAmount = -damageAmount;
	
	// Collide with enemy spawners when bullet is shot by an enemy, so the bullet can't damage the spawner
	if (isEnemySpawnerDamageable) _collisionBlockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER; // Do not collide with enemy spawners to allow bullets enter the enemy spawner position rectangle, so it can be damaged
	else _collisionBlockContent |= LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
}

BulletMovingEntity::~BulletMovingEntity() {}

int BulletMovingEntity::getDamageAmount()
{
	return _damageAmount;
}

int BulletMovingEntity::update()
{
	// Update position
	int movedPixelsCount = move(_facingDirection);
	
	// Did the bullet really moved ?
	if (movedPixelsCount == 0) return 1; // It did not move, so it has hit a wall
	
	// Is the bullet out of range ?
	_movedDistance += movedPixelsCount;
	if (_movedDistance >= _range) return 1; // The bullet is gone too far, destroy it
	
	return 0;
}
