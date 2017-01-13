#ifndef HPP_ENEMY_SPAWNER_ENTITY_HPP
#define HPP_ENEMY_SPAWNER_ENTITY_HPP

#include <Entity.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class EnemySpawnerEntity
 * A destructible still entity spawning enemies all around.
 * @author Adrien RICCIARDI
 */
class EnemySpawnerEntity: public Entity
{
	private:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** Hold the time when the last enemy was spawned. */
		unsigned int _lastEnemySpawnTime;
	
	public:
		/** Spawn an enemy spawner at the specified coordinates.
		 * @param x X coordinate in pixels.
		 * @param y Y coordinate in pixels.
		 */
		EnemySpawnerEntity(int x, int y): Entity(TextureManager::TEXTURE_ID_ENEMY_SPAWNER, x, y)
		{
			_lifePointsAmount = 100;
			_maximumLifePointsAmount = 100;
		}
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 */
		inline void modifyLife(int lifePointsAmount) // TODO duplicate with FightingEntity, but can't be moved to Entity because not all entities use life (i.e. bullet)
		{
			_lifePointsAmount += lifePointsAmount;
			
			// Clamp life points to [0; _maximumLifePointsAmount]
			if (_lifePointsAmount < 0) _lifePointsAmount = 0;
			else if (_lifePointsAmount > _maximumLifePointsAmount) _lifePointsAmount = _maximumLifePointsAmount;
		}
		
		/** Spawn an enemy if enough time elapsed and if there is a free block around the spawner.
		 * @return 1 if the spawner is destroyed and must be removed,
		 * @return 0 if the spawner must be kept.
		 */
		virtual int update()
		{
			// Remove the spawner if it is destroyed
			if (_lifePointsAmount == 0) return 1;

			return 0;
		}
};

#endif