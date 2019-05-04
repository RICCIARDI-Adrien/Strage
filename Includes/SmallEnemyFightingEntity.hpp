/** @file SmallEnemyFightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_SMALL_ENEMY_FIGHTING_ENTITY_HPP
#define HPP_SMALL_ENEMY_FIGHTING_ENTITY_HPP

#include <EnemyFightingEntity.hpp>

/** @class SmallEnemyFightingEntity
 * A small enemy moving quickly (but not as fast as the player) and shooting small caliber ammunition.
 */
class SmallEnemyFightingEntity: public EnemyFightingEntity
{
	public:
		/** Spawn a small enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		SmallEnemyFightingEntity(int x, int y);
		
		/** Free allocated resources. */
		virtual ~SmallEnemyFightingEntity();

	protected:
		// No need for documentation because it is the same as parent function
		virtual BulletMovingEntity *_fireBullet(int x, int y);
};

#endif
