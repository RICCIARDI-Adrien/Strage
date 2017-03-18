#ifndef HPP_FIGHTING_ENTITY_ENEMY_MEDIUM_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_MEDIUM_HPP

#include <AudioManager.hpp>
#include <FightingEntityEnemy.hpp>
#include <MovingEntityBulletMediumEnemy.hpp>
#include <TextureManager.hpp>

/** @class FightingEntityEnemyMedium
 * A medium enemy moving a little slower than small enemy but shooting bigger caliber ammunition.
 * @author Adrien RICCIARDI
 */
class FightingEntityEnemyMedium: public FightingEntityEnemy
{
	protected:
		/** Fire medium caliber ammunition.
		 * @param x Ammunition X coordinate.
		 * @param y Ammunition Y coordinate.
		 * @return The fired ammunition.
		 */
		virtual MovingEntityBullet *_fireBullet(int x, int y)
		{
			return new MovingEntityBulletMediumEnemy(x, y, _facingDirection);
		}
		
	public:
		/** Spawn a medium enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemyMedium(int x, int y): FightingEntityEnemy(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_FACING_UP, 2, 3, 2000, AudioManager::SOUND_ID_MEDIUM_ENEMY_FIRESHOT, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP) {}
		
		/** Free allocated resources. */
		virtual ~FightingEntityEnemyMedium() {}
};

#endif