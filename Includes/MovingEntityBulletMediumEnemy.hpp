#ifndef HPP_MOVING_ENTITY_BULLET_MEDIUM_ENEMY_HPP
#define HPP_MOVING_ENTITY_BULLET_MEDIUM_ENEMY_HPP

#include <MovingEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovingEntityBulletMediumEnemy
 * The bullets the medium enemies shoot.
 * @author Adrien RICCIARDI
 */
class MovingEntityBulletMediumEnemy: public MovingEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovingEntityBulletMediumEnemy(int x, int y, Direction facingDirection): MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET, 5, facingDirection, 3) {}
		
		/** Free allocated resources. */
		virtual ~MovingEntityBulletMediumEnemy() {}
};

#endif