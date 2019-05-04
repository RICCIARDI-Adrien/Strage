/** @file MovingEntityBullet.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MOVING_ENTITY_BULLET_HPP
#define HPP_MOVING_ENTITY_BULLET_HPP

#include <LevelManager.hpp> // TODO remove on refactoring
#include <MovingEntity.hpp>
#include <Renderer.hpp> // TODO remove on refactoring
#include <TextureManager.hpp>

/** @class MovingEntityBullet
 * A bullet fired by the player or the enemies.
 */
class MovingEntityBullet: public MovingEntity
{
	private:
		/** A bullet maximum traveling distance, the bullet will be destroyed if it has traveled this distance without colliding with something. */
		const int _range = 2 * Renderer::displayWidth;
		/** How many distance was traveled. */
		int _movedDistance;
		
		/** How many life points the bullet removes when it hits an entity. */
		int _damageAmount;
	
	public:
		/** Spawn a bullet.
		 * @param x Spawning X coordinate.
		 * @param y Spawning Y coordinate.
		 * @param textureId The texture to display.
		 * @param movingPixelsAmount Bullet moving speed in pixels.
		 * @param facingDirection In which direction the bullet will move.
		 * @param damageAmount How many life points the bullet removes when it hits an entity.
		 * @param isEnemySpawnerDamageable Set to 1 to make the bullet damage enemy spawers, set to 0 to disable enemy spawners damage dealing.
		 */
		MovingEntityBullet(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, Direction facingDirection, int damageAmount, int isEnemySpawnerDamageable): MovingEntity(x, y, textureId, movingPixelsAmount)
		{
			_facingDirection = facingDirection;
			_movedDistance = 0;
			_damageAmount = -damageAmount;
			
			// Collide with enemy spawners when bullet is shot by an enemy, so the bullet can't damage the spawner
			if (isEnemySpawnerDamageable) _collisionBlockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER; // Do not collide with enemy spawners to allow bullets enter the enemy spawner position rectangle, so it can be damaged
			else _collisionBlockContent |= LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
		}
		
		/** Free allocated resources. */
		virtual ~MovingEntityBullet() {}
		
		/** How many life points the bullet removes.
		 * @return Life points to ADD to hit entity (the value is negative yet to directly use with modifyLife() functions).
		 */
		int getDamageAmount()
		{
			return _damageAmount;
		}
		
		/** Check if a scene wall has been hit.
		 * @return 0 if nothing was hit,
		 * @return 1 if scene wall was hit and the bullet must be destroyed.
		 * @warning This function does not check against enemy or player collision.
		 */
		virtual int update()
		{
			int movedPixelsCount = 0;
			
			// Update position
			movedPixelsCount = move(_facingDirection);
			
			// Did the bullet really moved ?
			if (movedPixelsCount == 0) return 1; // It did not move, so it has hit a wall
			
			// Is the bullet out of range ?
			_movedDistance += movedPixelsCount;
			if (_movedDistance >= _range) return 1; // The bullet is gone too far, destroy it
			
			return 0;
		}
};

#endif
