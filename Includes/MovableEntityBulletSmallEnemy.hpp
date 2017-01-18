#ifndef HPP_MOVABLE_ENTITY_BULLET_SMALL_ENEMY_HPP
#define HPP_MOVABLE_ENTITY_BULLET_SMALL_ENEMY_HPP

#include <MovableEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovableEntityBulletSmallEnemy
 * The bullets the small enemies shoot.
 * @author Adrien RICCIARDI
 */
class MovableEntityBulletSmallEnemy: public MovableEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovableEntityBulletSmallEnemy(int x, int y, Direction facingDirection): MovableEntityBullet(x, y, TextureManager::TEXTURE_ID_SMALL_ENEMY_BULLET, 6, facingDirection, 1) {}
};

#endif