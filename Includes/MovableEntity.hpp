#ifndef HPP_MOVABLE_ENTITY_HPP
#define HPP_MOVABLE_ENTITY_HPP

#include <Entity.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class MovableEntity
 * An entity that can move up, down, left and right onto the map. A movable entity can't go across a scene wall.
 * @author Adrien RICCIARDI
 */
class MovableEntity: public Entity
{
	public:
		/** All entity possible facing directions. */
		typedef enum
		{
			FACING_DIRECTION_UP,
			FACING_DIRECTION_DOWN,
			FACING_DIRECTION_LEFT,
			FACING_DIRECTION_RIGHT
		} FacingDirection; // Must be declared before usage in protected section
		
	protected:
		/** Rotate the texture when rendering (degrees unit). */
		double _rotationAngle;
		/** Tell in which direction the entity is facing. */
		FacingDirection _facingDirection;
		
		/** How many pixels to move the entity. */
		int _movingPixelsAmount;
	
	public:
		/** Gather some initialization common to all movable entities.
		 * @param textureId The texture to use on rendering.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param movingPixelsAmount Entity moving speed.
		 */
		MovableEntity(TextureManager::TextureId textureId, int x, int y, int movingPixelsAmount): Entity(textureId) // TODO add facing direction here ?
		{
			// Cache some parameters to fasten rendering
			_renderingDestinationRectangle.x = x;
			_renderingDestinationRectangle.y = y;
			
			_movingPixelsAmount = movingPixelsAmount;
			
			// Entity is facing up on spawn
			_rotationAngle = 0;
			_facingDirection = FACING_DIRECTION_UP;
		}
		
		/** Move the entity to the up. */
		virtual void moveToUp()
		{
			int leftSideDistanceToWall, rightSizeDistanceToWall, distanceToWall;
			
			// Check upper border distance against level walls
			leftSideDistanceToWall = LevelManager::getDistanceFromUpperWall(_renderingDestinationRectangle.x, _renderingDestinationRectangle.y);
			rightSizeDistanceToWall = LevelManager::getDistanceFromUpperWall(_renderingDestinationRectangle.x + _renderingDestinationRectangle.w - 1, _renderingDestinationRectangle.y);
			
			// Keep the smaller distance
			if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
			else distanceToWall = rightSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) _renderingDestinationRectangle.y -= _movingPixelsAmount;
			else _renderingDestinationRectangle.y -= distanceToWall;
			
			// Entity is facing up
			_rotationAngle = 0;
			_facingDirection = FACING_DIRECTION_UP;
		}
		
		/** Move the entity to the down. */
		virtual void moveToDown()
		{
			int leftSideDistanceToWall, rightSizeDistanceToWall, distanceToWall;
			
			// Check downer border distance against level walls
			leftSideDistanceToWall = LevelManager::getDistanceFromDownerWall(_renderingDestinationRectangle.x, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h);
			rightSizeDistanceToWall = LevelManager::getDistanceFromDownerWall(_renderingDestinationRectangle.x + _renderingDestinationRectangle.w - 1, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h);
			
			// Keep the smaller distance
			if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
			else distanceToWall = rightSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) _renderingDestinationRectangle.y += _movingPixelsAmount;
			else _renderingDestinationRectangle.y += distanceToWall;
			
			// Entity is facing down
			_rotationAngle = 180;
			_facingDirection = FACING_DIRECTION_DOWN;
		}
		
		/** Move the entity to the left. */
		virtual void moveToLeft()
		{
			int upperSideDistanceToWall, downerSizeDistanceToWall, distanceToWall;
			
			// Check leftmost border distance against level walls
			upperSideDistanceToWall = LevelManager::getDistanceFromLeftmostWall(_renderingDestinationRectangle.x, _renderingDestinationRectangle.y);
			downerSizeDistanceToWall = LevelManager::getDistanceFromLeftmostWall(_renderingDestinationRectangle.x, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h - 1);
			
			// Keep the smaller distance
			if (upperSideDistanceToWall < downerSizeDistanceToWall) distanceToWall = upperSideDistanceToWall;
			else distanceToWall = downerSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) _renderingDestinationRectangle.x -= _movingPixelsAmount;
			else _renderingDestinationRectangle.x -= distanceToWall;
			
			// Entity is facing left
			_rotationAngle = 270;
			_facingDirection = FACING_DIRECTION_LEFT;
		}
		
		/** Move the entity to the right. */
		virtual void moveToRight()
		{
			int upperSideDistanceToWall, downerSizeDistanceToWall, distanceToWall;
			
			// Check rightmost border distance against level walls
			upperSideDistanceToWall = LevelManager::getDistanceFromRightmostWall(_renderingDestinationRectangle.x + _renderingDestinationRectangle.w, _renderingDestinationRectangle.y);
			downerSizeDistanceToWall = LevelManager::getDistanceFromRightmostWall(_renderingDestinationRectangle.x + _renderingDestinationRectangle.w, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h - 1);
			
			// Keep the smaller distance
			if (upperSideDistanceToWall < downerSizeDistanceToWall) distanceToWall = upperSideDistanceToWall;
			else distanceToWall = downerSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) _renderingDestinationRectangle.x += _movingPixelsAmount;
			else _renderingDestinationRectangle.x += distanceToWall;
			
			// Entity is facing right
			_rotationAngle = 90;
			_facingDirection = FACING_DIRECTION_RIGHT;
		}
		
		/** Display the texture and rotate it in the same time. */
		virtual void render()
		{
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(&_renderingDestinationRectangle)) _pointerTexture->render(_renderingDestinationRectangle.x - Renderer::displayX, _renderingDestinationRectangle.y - Renderer::displayY, _rotationAngle);
		}
		
		/** Tell in which direction the entity is facing.
		 * @return The entity facing direction.
		 */
		inline FacingDirection getFacingDirection()
		{
			return _facingDirection;
		}
};

#endif