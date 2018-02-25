#ifndef HPP_MOVING_ENTITY_BULLET_PLAYER_MORTAR_SHELL_HPP
#define HPP_MOVING_ENTITY_BULLET_PLAYER_MORTAR_SHELL_HPP

#include <LevelManager.hpp>
#include <MovingEntityBullet.hpp>
#include <TextureManager.hpp>

/** @class MovingEntityBulletPlayerMortarShell
 * The player secondary shoot.
 * @author Adrien RICCIARDI
 */
class MovingEntityBulletPlayerMortarShell: public MovingEntityBullet
{
	public:
		/** Spawn a mortar shell.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovingEntityBulletPlayerMortarShell(int x, int y, Direction facingDirection): MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP, 4, facingDirection, 20)
		{
			// Do not collide with enemy spawners to allow bullets enter the enemy spawner position rectangle, so it can be damaged
			_collisionBlockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
		}
		
		/** @overload */
		virtual ~MovingEntityBulletPlayerMortarShell() {}
};

#endif
