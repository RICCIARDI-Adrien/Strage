/** @file EnemySpawnerStaticEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_ENEMY_SPAWNER_STATIC_ENTITY_HPP
#define HPP_ENEMY_SPAWNER_STATIC_ENTITY_HPP

#include <AnimatedTexture.hpp>
#include <StaticEntity.hpp>

/** @class EnemySpawnerStaticEntity
 * A destructible still entity spawning enemies all around.
 */
class EnemySpawnerStaticEntity: public StaticEntity
{
	public:
		/** Spawn an enemy spawner at the specified coordinates.
		 * @param x X coordinate in pixels.
		 * @param y Y coordinate in pixels.
		 */
		EnemySpawnerStaticEntity(int x, int y);
		
		// No need for documentation because it is the same as parent function
		virtual ~EnemySpawnerStaticEntity();
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 */
		inline void modifyLife(int lifePointsAmount) // TODO duplicate with FightingEntity, but can't be moved to Entity because not all entities use life (i.e. bullet)
		{
			_lifePointsAmount += lifePointsAmount;
			
			// Make sure life points do not become negative
			if (_lifePointsAmount < 0) _lifePointsAmount = 0;
		}
		
		// No need for documentation because it is the same as parent function
		virtual void render();
		
		/** Handle only spawner life state.
		 * @return 0 if the spawner must be kept,
		 * @return 1 if the spawner is destroyed and must be removed.
		 */
		virtual int update();

	private:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		
		/** The animation shown in the middle of the spawner. Using a separate texture allows to change the spawner texture without interfering with the animation. */
		AnimatedTexture *_pointerEffectTexture;
};

#endif
