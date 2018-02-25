/** @file FightingEntityEnemyBig.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_FIGHTING_ENTITY_ENEMY_BIG_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_BIG_HPP

#include <EffectManager.hpp>
#include <FightingEntityEnemy.hpp>
#include <MovingEntityBulletBigEnemy.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntityEnemyBig
 * A big enemy moving slowly and shooting huge caliber ammunition.
 */
class FightingEntityEnemyBig: public FightingEntityEnemy
{
	protected:
		// No need for documentation because it is the same as parent function
		virtual MovingEntityBullet *_fireBullet(int x, int y)
		{
			return new MovingEntityBulletBigEnemy(x, y, _facingDirection);
		}
		
	public:
		/** Spawn a big enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemyBig(int x, int y): FightingEntityEnemy(x, y, TextureManager::TEXTURE_ID_BIG_ENEMY_FACING_UP, 1, 10, 10000, TextureManager::TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP, EffectManager::EFFECT_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP, EffectManager::EFFECT_ID_BIG_ENEMY_EXPLOSION)
		{
			unsigned int currentTime;
			
			// Make big enemies wait 3 seconds before shooting (to avoid them shooting instantly when spawned)
			currentTime = SDL_GetTicks();
			if (currentTime > 7000) _lastShotTime = currentTime - 7000;
		}
		
		/** Free allocated resources. */
		virtual ~FightingEntityEnemyBig() {}
};

#endif
