/** @file FightingEntityEnemyMedium.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_FIGHTING_ENTITY_ENEMY_MEDIUM_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_MEDIUM_HPP

#include <EffectManager.hpp>
#include <FightingEntityEnemy.hpp>
#include <TextureManager.hpp>

/** @class FightingEntityEnemyMedium
 * A medium enemy moving a little slower than small enemy but shooting bigger caliber ammunition.
 */
class FightingEntityEnemyMedium: public FightingEntityEnemy
{
	protected:
		// No need for documentation because it is the same as parent function
		virtual MovingEntityBullet *_fireBullet(int x, int y)
		{
			return new MovingEntityBullet(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP, 5, _facingDirection, 3, 0);
		}
		
	public:
		/** Spawn a medium enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemyMedium(int x, int y): FightingEntityEnemy(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_FACING_UP, 2, 3, 2000, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP, EffectManager::EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP, EffectManager::EFFECT_ID_MEDIUM_ENEMY_EXPLOSION) {}
		
		/** Free allocated resources. */
		virtual ~FightingEntityEnemyMedium() {}
};

#endif
