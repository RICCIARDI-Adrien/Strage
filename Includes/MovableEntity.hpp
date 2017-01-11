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
		/** All entity possible directions. */
		typedef enum
		{
			DIRECTION_UP,
			DIRECTION_DOWN,
			DIRECTION_LEFT,
			DIRECTION_RIGHT,
			DIRECTIONS_COUNT
		} Direction; // Must be declared before usage in protected section
		
	protected:
		/** Rotate the texture when rendering (degrees unit). */
		double _rotationAngle;
		/** Tell in which direction the entity is facing. */
		Direction _facingDirection;
		
		/** How many pixels to move the entity. */
		int _movingPixelsAmount;
	
	public:
		/** Gather some initialization common to all movable entities.
		 * @param textureId The texture to use on rendering.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param movingPixelsAmount Entity moving speed.
		 */
		MovableEntity(TextureManager::TextureId textureId, int x, int y, int movingPixelsAmount): Entity(textureId, x, y)
		{
			_movingPixelsAmount = movingPixelsAmount;
			
			// Entity is facing up on spawn
			_rotationAngle = 0;
			_facingDirection = DIRECTION_UP;
		}
		
		/** Move the entity to the up.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToUp()
		{
			int leftSideDistanceToWall, rightSizeDistanceToWall, distanceToWall, movingPixelsAmount;
			
			// Check upper border distance against level walls
			leftSideDistanceToWall = LevelManager::getDistanceFromUpperWall(_positionRectangle.x, _positionRectangle.y);
			rightSizeDistanceToWall = LevelManager::getDistanceFromUpperWall(_positionRectangle.x + _positionRectangle.w - 1, _positionRectangle.y);
			
			// Keep the smaller distance
			if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
			else distanceToWall = rightSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			_positionRectangle.y -= movingPixelsAmount;
			
			// Entity is facing up
			_rotationAngle = 0;
			_facingDirection = DIRECTION_UP;
			
			return movingPixelsAmount;
		}
		
		/** Move the entity to the down.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToDown()
		{
			int leftSideDistanceToWall, rightSizeDistanceToWall, distanceToWall, movingPixelsAmount;
			
			// Check downer border distance against level walls
			leftSideDistanceToWall = LevelManager::getDistanceFromDownerWall(_positionRectangle.x, _positionRectangle.y + _positionRectangle.h);
			rightSizeDistanceToWall = LevelManager::getDistanceFromDownerWall(_positionRectangle.x + _positionRectangle.w - 1, _positionRectangle.y + _positionRectangle.h);
			
			// Keep the smaller distance
			if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
			else distanceToWall = rightSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			_positionRectangle.y += movingPixelsAmount;
			
			// Entity is facing down
			_rotationAngle = 180;
			_facingDirection = DIRECTION_DOWN;
			
			return movingPixelsAmount;
		}
		
		/** Move the entity to the left.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToLeft()
		{
			int upperSideDistanceToWall, downerSideDistanceToWall, distanceToWall, movingPixelsAmount;
			
			// Check leftmost border distance against level walls
			upperSideDistanceToWall = LevelManager::getDistanceFromLeftmostWall(_positionRectangle.x, _positionRectangle.y);
			downerSideDistanceToWall = LevelManager::getDistanceFromLeftmostWall(_positionRectangle.x, _positionRectangle.y + _positionRectangle.h - 1);
			
			// Keep the smaller distance
			if (upperSideDistanceToWall < downerSideDistanceToWall) distanceToWall = upperSideDistanceToWall;
			else distanceToWall = downerSideDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			_positionRectangle.x -= movingPixelsAmount;
			
			// Entity is facing left
			_rotationAngle = 270;
			_facingDirection = DIRECTION_LEFT;
			
			return movingPixelsAmount;
		}
		
		/** Move the entity to the right.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToRight()
		{
			int upperSideDistanceToWall, downerSideDistanceToWall, distanceToWall, movingPixelsAmount;
			
			// Check rightmost border distance against level walls
			upperSideDistanceToWall = LevelManager::getDistanceFromRightmostWall(_positionRectangle.x + _positionRectangle.w, _positionRectangle.y);
			downerSideDistanceToWall = LevelManager::getDistanceFromRightmostWall(_positionRectangle.x + _positionRectangle.w, _positionRectangle.y + _positionRectangle.h - 1);
			
			// Keep the smaller distance
			if (upperSideDistanceToWall < downerSideDistanceToWall) distanceToWall = upperSideDistanceToWall;
			else distanceToWall = downerSideDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			_positionRectangle.x += movingPixelsAmount;
			
			// Entity is facing right
			_rotationAngle = 90;
			_facingDirection = DIRECTION_RIGHT;
			
			return movingPixelsAmount;
		}
		
		/** Make the entity move at its speed to the specified direction.
		 * @param movingDirection The direction to move to.
		 * @return How many pixels the entity moved.
		 */
		virtual int move(Direction movingDirection)
		{
			switch (movingDirection)
			{
				case DIRECTION_UP:
					return moveToUp();
					
				case DIRECTION_DOWN:
					return moveToDown();
					
				case DIRECTION_LEFT:
					return moveToLeft();
					
				case DIRECTION_RIGHT:
					return moveToRight();
					
				default:
					return 0; // Tell that the entity did not move
			}
		}
		
		/** Turn the entity to the up direction. */
		void turnToUp()
		{
			_rotationAngle = 0;
			_facingDirection = DIRECTION_UP;
		}
		
		/** Turn the entity to the down direction. */
		void turnToDown()
		{
			_rotationAngle = 180;
			_facingDirection = DIRECTION_DOWN;
		}
		
		/** Turn the entity to the left direction. */
		void turnToLeft()
		{
			_rotationAngle = 270;
			_facingDirection = DIRECTION_LEFT;
		}
		
		/** Turn the entity to the right direction. */
		void turnToRight()
		{
			_rotationAngle = 90;
			_facingDirection = DIRECTION_RIGHT;
		}
		
		/** Display the texture and rotate it in the same time. */
		virtual void render()
		{
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(&_positionRectangle)) _pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY, _rotationAngle);
		}
		
		/** Tell in which direction the entity is facing.
		 * @return The entity facing direction.
		 */
		inline Direction getFacingDirection()
		{
			return _facingDirection;
		}
};

#endif