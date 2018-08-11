/** @file StaticEntityEnemySpawner.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_STATIC_ENTITY_ENEMY_SPAWNER_HPP
#define HPP_STATIC_ENTITY_ENEMY_SPAWNER_HPP

#include <StaticEntity.hpp>

/** @class StaticEntityEnemySpawner
 * A destructible still entity spawning enemies all around.
 */
class StaticEntityEnemySpawner: public StaticEntity
{
	private:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		
	public:
		/** Spawn an enemy spawner at the specified coordinates.
		 * @param x X coordinate in pixels.
		 * @param y Y coordinate in pixels.
		 */
		StaticEntityEnemySpawner(int x, int y);
		
		// No need for documentation because it is the same as parent function
		virtual ~StaticEntityEnemySpawner() {}
		
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
		virtual int update();
};

#endif
