/** @file BigEnemyFightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_BIG_ENEMY_FIGHTING_ENTITY_HPP
#define HPP_BIG_ENEMY_FIGHTING_ENTITY_HPP

#include <EnemyFightingEntity.hpp>

/** @class BigEnemyFightingEntity
 * A big enemy moving slowly and shooting huge caliber ammunition.
 */
class BigEnemyFightingEntity: public EnemyFightingEntity
{
	public:
		/** Spawn a big enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		BigEnemyFightingEntity(int x, int y);
		
		/** Free allocated resources. */
		virtual ~BigEnemyFightingEntity();
		
		// No need for documentation because it is the same as parent function
		virtual int update();

	protected:
		// No need for documentation because it is the same as parent function
		virtual BulletMovingEntity *_fireBullet(int x, int y);
};

#endif
