#ifndef HPP_MOVING_ENTITY_BULLET_SMALL_ENEMY_HPP
#define HPP_MOVING_ENTITY_BULLET_SMALL_ENEMY_HPP

#include <MovingEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovingEntityBulletSmallEnemy
 * The bullets the small enemies shoot.
 * @author Adrien RICCIARDI
 */
class MovingEntityBulletSmallEnemy: public MovingEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovingEntityBulletSmallEnemy(int x, int y, Direction facingDirection): MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP, 6, facingDirection, 1) {}
		
		/** Free allocated resources. */
		virtual ~MovingEntityBulletSmallEnemy() {}
};

#endif