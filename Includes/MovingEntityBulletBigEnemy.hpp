#ifndef HPP_MOVING_ENTITY_BULLET_BIG_ENEMY_HPP
#define HPP_MOVING_ENTITY_BULLET_BIG_ENEMY_HPP

#include <MovingEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovingEntityBulletBigEnemy
 * The bullets the big enemies shoot.
 * @author Adrien RICCIARDI
 */
class MovingEntityBulletBigEnemy: public MovingEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovingEntityBulletBigEnemy(int x, int y, Direction facingDirection): MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP, 4, facingDirection, 20) {}
		
		/** Free allocated resources. */
		virtual ~MovingEntityBulletBigEnemy() {}
};

#endif