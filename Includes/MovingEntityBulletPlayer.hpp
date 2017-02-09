#ifndef HPP_MOVING_ENTITY_BULLET_PLAYER_HPP
#define HPP_MOVING_ENTITY_BULLET_PLAYER_HPP

#include <LevelManager.hpp>
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
		MovingEntityBulletPlayer(int x, int y, Direction facingDirection): MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_PLAYER_BULLET_FACING_UP, 6, facingDirection, 1)
		{
			// Do not collide with enemy spawners to allow bullets enter the enemy spawner position rectangle, so it can be damaged
			_collisionBlockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
		}
		
		/** Free allocated resources. */
		virtual ~MovingEntityBulletPlayer() {}
};

#endif