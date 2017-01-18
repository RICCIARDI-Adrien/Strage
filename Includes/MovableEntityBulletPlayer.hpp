#ifndef HPP_MOVABLE_ENTITY_BULLET_PLAYER_HPP
#define HPP_MOVABLE_ENTITY_BULLET_PLAYER_HPP

#include <MovableEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovableEntityBulletPlayer
 * The bullets the player shoots.
 * @author Adrien RICCIARDI
 */
class MovableEntityBulletPlayer: public MovableEntityBullet
{
	public:
		/** Spawn a bullet.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovableEntityBulletPlayer(int x, int y, Direction facingDirection): MovableEntityBullet(x, y, TextureManager::TEXTURE_ID_PLAYER_BULLET, 6, facingDirection, 1) {}
};

#endif