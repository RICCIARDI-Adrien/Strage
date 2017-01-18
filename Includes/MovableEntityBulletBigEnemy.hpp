#ifndef HPP_MOVABLE_ENTITY_BULLET_BIG_ENEMY_HPP
#define HPP_MOVABLE_ENTITY_BULLET_BIG_ENEMY_HPP

#include <MovableEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovableEntityBulletBigEnemy
 * The bullets the big enemies shoot.
 * @author Adrien RICCIARDI
 */
class MovableEntityBulletBigEnemy: public MovableEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovableEntityBulletBigEnemy(int x, int y, Direction facingDirection): MovableEntityBullet(x, y, TextureManager::TEXTURE_ID_BIG_ENEMY_BULLET, 4, facingDirection, 20) {}
};

#endif