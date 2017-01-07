#ifndef HPP_FIGHTING_ENTITY_HPP
#define HPP_FIGHTING_ENTITY_HPP

#include <MovableEntity.hpp>
#include <TextureManager.hpp>

/** @class FightingEntity
 * A movable entity that has ammunitions and life points.
 * @author Adrien RICCIARDI
 */
class FightingEntity: public MovableEntity
{
	protected:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** How many ammunitions the entity owns. */
		int _ammunitionsAmount;
		
	public:
		/** Initialize life points in addition to parent classes fields.
		 * @param textureId The texture to use on rendering.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param movingPixelsAmount Entity moving speed.
		 * @param maximumLifePointsAmount Entity maximum life points count.
		 */
		FightingEntity(TextureManager::TextureId textureId, int x, int y, int movingPixelsAmount, int maximumLifePointsAmount): MovableEntity(textureId, x, y, movingPixelsAmount)
		{
			_lifePointsAmount = maximumLifePointsAmount;
			_maximumLifePointsAmount = maximumLifePointsAmount;
		}
		
		/** The the entity life points.
		 * @return Entity life point amount.
		 */
		inline int getLifePointsAmount()
		{
			return _lifePointsAmount;
		}
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 */
		inline void modifyLife(int lifePointsAmount)
		{
			_lifePointsAmount += lifePointsAmount;
			
			// Clamp life points to [0; _maximumLifePointsAmount]
			if (_lifePointsAmount < 0) _lifePointsAmount = 0;
			else if (_lifePointsAmount > _maximumLifePointsAmount) _lifePointsAmount = _maximumLifePointsAmount;
		}
		
		/** Tell if the entity is at maximum life or not.
		 * @return 1 if the entity can be healed,
		 * @return 0 if the entity is healthy.
		 */
		inline int isWounded()
		{
			if (_lifePointsAmount < _maximumLifePointsAmount) return 1;
			return 0;
		}
};

#endif