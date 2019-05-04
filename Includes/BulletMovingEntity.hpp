/** @file BulletMovingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_BULLET_MOVING_ENTITY_HPP
#define HPP_BULLET_MOVING_ENTITY_HPP

#include <MovingEntity.hpp>

/** @class BulletMovingEntity
 * A bullet fired by the player or the enemies.
 */
class BulletMovingEntity: public MovingEntity
{
	public:
		/** Spawn a bullet.
		 * @param x Spawning X coordinate.
		 * @param y Spawning Y coordinate.
		 * @param textureId The texture to display.
		 * @param movingPixelsAmount Bullet moving speed in pixels.
		 * @param facingDirection In which direction the bullet will move.
		 * @param damageAmount How many life points the bullet removes when it hits an entity.
		 * @param isEnemySpawnerDamageable Set to true to make the bullet damage enemy spawers, set to false to disable enemy spawners damage dealing.
		 */
		BulletMovingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, Direction facingDirection, int damageAmount, bool isEnemySpawnerDamageable);
		
		/** Free allocated resources. */
		virtual ~BulletMovingEntity();
		
		/** How many life points the bullet removes.
		 * @return Life points to ADD to hit entity (the value is negative yet to directly use with modifyLife() functions).
		 */
		int getDamageAmount();
		
		/** Check if a scene wall has been hit.
		 * @return 0 if nothing was hit,
		 * @return 1 if scene wall was hit and the bullet must be destroyed.
		 * @warning This function does not check against enemy or player collision.
		 */
		virtual int update();

	private:
		/** A bullet maximum traveling distance, the bullet will be destroyed if it has traveled this distance without colliding with something. */
		int _range;
		/** How many distance was traveled. */
		int _movedDistance;
		
		/** How many life points the bullet removes when it hits an entity. */
		int _damageAmount;
};

#endif
