#ifndef HPP_MOVABLE_ENTITY_BULLET_HPP
#define HPP_MOVABLE_ENTITY_BULLET_HPP

#include <MovableEntity.hpp>
#include <TextureManager.hpp>

/** @class MovableEntityBullet
 * A bullet fired by the player or the enemies.
 * @author Adrien RICCIARDI
 */
class MovableEntityBullet: public MovableEntity
{
	public:
		/** Spawn a bullet.
		 * @param x Spawning X coordinate.
		 * @param y Spawning Y coordinate.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovableEntityBullet(int x, int y, Direction facingDirection): MovableEntity(TextureManager::TEXTURE_ID_BULLET, x, y, 6)
		{
			_facingDirection = facingDirection;
		}
		
		/** Free entity allocated resources. */
		virtual ~MovableEntityBullet() {} // Nothing to free
		
		/** Check if a scene wall has been hit.
		 * @return 0 if nothing was hit,
		 * @return 1 if scene wall was hit and the bullet must be destroyed.
		 * @warning This function does not check against enemy or player collision.
		 */
		virtual int update()
		{
			int movedPixelsCount = 0;
		
			// Update position
			movedPixelsCount = move(_facingDirection);
			
			// Did the bullet really moved ?
			if (movedPixelsCount == 0) return 1; // It did not move, so it has hit a wall
			return 0;
		}
};

#endif