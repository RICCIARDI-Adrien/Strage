#ifndef HPP_FIGHTING_ENTITY_ENEMY_SMALL_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_SMALL_HPP

#include <AudioManager.hpp>
#include <FightingEntityEnemy.hpp>
#include <MovingEntityBulletSmallEnemy.hpp>
#include <TextureManager.hpp>

/** @class FightingEntityEnemySmall
 * A small enemy moving quickly (but no so fast as the player) and shooting small caliber ammunition.
 * @author Adrien RICCIARDI
 */
class FightingEntityEnemySmall: public FightingEntityEnemy
{
	protected:
		/** Fire small caliber ammunition.
		 * @param x Ammunition X coordinate.
		 * @param y Ammunition Y coordinate.
		 * @return The fired ammunition.
		 */
		virtual MovingEntityBullet *_fireBullet(int x, int y)
		{
			return new MovingEntityBulletSmallEnemy(x, y, _facingDirection);
		}
		
	public:
		/** Spawn a small enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemySmall(int x, int y): FightingEntityEnemy(x, y, TextureManager::TEXTURE_ID_SMALL_ENEMY_FACING_UP, 2, 1, 1000, AudioManager::SOUND_ID_SMALL_ENEMY_FIRESHOT) {}
		
		/** Free allocated resources. */
		virtual ~FightingEntityEnemySmall() {}
};

#endif