#ifndef HPP_MOVING_ENTITY_BULLET_PLAYER_HPP
#define HPP_MOVING_ENTITY_BULLET_PLAYER_HPP

#include <MovingEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovingEntityBulletPlayer
 * The bullets the player shoots.
 * @author Adrien RICCIARDI
 */
class MovingEntityBulletPlayer: public MovingEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovingEntityBulletPlayer(int x, int y, Direction facingDirection): MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_PLAYER_BULLET, 6, facingDirection, 1) {}
};

#endif