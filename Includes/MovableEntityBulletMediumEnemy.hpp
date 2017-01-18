#ifndef HPP_MOVABLE_ENTITY_BULLET_MEDIUM_ENEMY_HPP
#define HPP_MOVABLE_ENTITY_BULLET_MEDIUM_ENEMY_HPP

#include <MovableEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovableEntityBulletMediumEnemy
 * The bullets the medium enemies shoot.
 * @author Adrien RICCIARDI
 */
class MovableEntityBulletMediumEnemy: public MovableEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovableEntityBulletMediumEnemy(int x, int y, Direction facingDirection): MovableEntityBullet(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET, 5, facingDirection, 3) {}
};

#endif