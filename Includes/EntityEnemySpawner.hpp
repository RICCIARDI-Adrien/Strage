#ifndef HPP_ENTITY_ENEMY_SPAWNER_HPP
#define HPP_ENTITY_ENEMY_SPAWNER_HPP

#include <SDL2/SDL.h>
#include <StaticEntity.hpp>
#include <TextureManager.hpp>

/** @class EntityEnemySpawner
 * A destructible still entity spawning enemies all around.
 * @author Adrien RICCIARDI
 */
class EntityEnemySpawner: public StaticEntity
{
	private:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		
	public:
		/** Spawn an enemy spawner at the specified coordinates.
		 * @param x X coordinate in pixels.
		 * @param y Y coordinate in pixels.
		 */
		EntityEnemySpawner(int x, int y): StaticEntity(x, y, TextureManager::TEXTURE_ID_ENEMY_SPAWNER)
		{
			_lifePointsAmount = 10;
		}
		
		/** Free allocated resources. */
		virtual ~EntityEnemySpawner() {};
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 */
		inline void modifyLife(int lifePointsAmount) // TODO duplicate with FightingEntity, but can't be moved to Entity because not all entities use life (i.e. bullet)
		{
			_lifePointsAmount += lifePointsAmount;
			
			// Make sure life points do not become negative
			if (_lifePointsAmount < 0) _lifePointsAmount = 0;
		}
		
		/** Handle only spawner life state.
		 * @return 0 if the spawner must be kept,
		 * @return 1 if the spawner is destroyed and must be removed.
		 */
		virtual int update()
		{
			// Remove the spawner if it is destroyed
			if (_lifePointsAmount == 0) return 1;
			return 0;
		}
};

#endif