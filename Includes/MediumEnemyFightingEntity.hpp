/** @file MediumEnemyFightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MEDIUM_ENEMY_FIGHTING_ENTITY_HPP
#define HPP_MEDIUM_ENEMY_FIGHTING_ENTITY_HPP

#include <EnemyFightingEntity.hpp>

/** @class MediumEnemyFightingEntity
 * A medium enemy moving a little slower than small enemy but shooting bigger caliber ammunition.
 */
class MediumEnemyFightingEntity: public EnemyFightingEntity
{
	public:
		/** Spawn a medium enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		MediumEnemyFightingEntity(int x, int y);
		
		/** Free allocated resources. */
		virtual ~MediumEnemyFightingEntity();
		
		// No need for documentation because it is the same as parent function
		virtual int update();

	protected:
		// No need for documentation because it is the same as parent function
		virtual BulletMovingEntity *_fireBullet(int x, int y);
};

#endif
