#ifndef HPP_FIGHTING_ENTITY_ENEMY_BIG_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_BIG_HPP

#include <AudioManager.hpp>
#include <FightingEntityEnemy.hpp>
#include <MovableEntityBullet.hpp> // TODO replace with real one
#include <TextureManager.hpp>

/** @class FightingEntityEnemyBig
 * A big enemy moving slowly and shooting huge caliber ammunition.
 * @author Adrien RICCIARDI
 */
class FightingEntityEnemyBig: public FightingEntityEnemy
{
	protected:
		/** Fire big caliber ammunition.
		 * @param x Ammunition X coordinate.
		 * @param y Ammunition Y coordinate.
		 * @return The fired ammunition.
		 */
		virtual MovableEntityBullet *_fireBullet(int x, int y)
		{
			return new MovableEntityBullet(x, y, _facingDirection);
		}
		
	public:
		/** Spawn a big enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemyBig(int x, int y): FightingEntityEnemy(x, y, TextureManager::TEXTURE_ID_BIG_ENEMY, 1, 10, 10000, AudioManager::SOUND_ID_BIG_ENEMY_FIRESHOT) {}
};

#endif