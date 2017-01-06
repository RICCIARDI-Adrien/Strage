#ifndef HPP_MOVABLE_ENTITY_HPP
#define HPP_MOVABLE_ENTITY_HPP

#include <Entity.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

class MovableEntity: public Entity
{
	protected:
		// TODO collision rectangle (pickabilility, impacts)
		
		/** Rotate the texture when rendering (degrees unit). */
		double _rotationAngle;
		
		/** How many pixels to move the entity. */
		int _movingPixelsAmount;
	
	public:
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
			
			//printf("up (%d,%d)=%d, (%d,%d)=%d, dist=%d\n", _renderingDestinationRectangle.x, _renderingDestinationRectangle.y, leftSideDistanceToWall, _renderingDestinationRectangle.x + _renderingDestinationRectangle.w - 1, _renderingDestinationRectangle.y, rightSizeDistanceToWall, distanceToWall);
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount)
			{
				_y -= _movingPixelsAmount;
				_renderingDestinationRectangle.y -= _movingPixelsAmount;
			}
			else
			{
				_y -= distanceToWall;
				_renderingDestinationRectangle.y -= distanceToWall;
			}
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
			
			//printf("down (%d,%d)=%d, (%d,%d)=%d dist=%d\n", _renderingDestinationRectangle.x, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h, leftSideDistanceToWall, _renderingDestinationRectangle.x + _renderingDestinationRectangle.w - 1, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h, rightSizeDistanceToWall, distanceToWall);
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount)
			{
				_y += _movingPixelsAmount;
				_renderingDestinationRectangle.y += _movingPixelsAmount;
			}
			else
			{
				_y += distanceToWall;
				_renderingDestinationRectangle.y += distanceToWall;
			}
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
			
			//printf("left (%d,%d)=%d, (%d,%d)=%d dist=%d\n", _renderingDestinationRectangle.x, _renderingDestinationRectangle.y, upperSideDistanceToWall, _renderingDestinationRectangle.x, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h, downerSizeDistanceToWall, distanceToWall);
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount)
			{
				_x -= _movingPixelsAmount;
				_renderingDestinationRectangle.x -= _movingPixelsAmount;
			}
			else
			{
				_x -= distanceToWall;
				_renderingDestinationRectangle.x -= distanceToWall;
			}
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
			
			//printf("right (%d,%d)=%d, (%d,%d)=%d dist=%d\n", _renderingDestinationRectangle.x + _renderingDestinationRectangle.w - 1, _renderingDestinationRectangle.y, upperSideDistanceToWall, _renderingDestinationRectangle.x + _renderingDestinationRectangle.w - 1, _renderingDestinationRectangle.y + _renderingDestinationRectangle.h -1, downerSizeDistanceToWall, distanceToWall);
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount)
			{
				_x += _movingPixelsAmount;
				_renderingDestinationRectangle.x += _movingPixelsAmount;
			}
			else
			{
				_x += distanceToWall;
				_renderingDestinationRectangle.x += distanceToWall;
			}
		}
		
		/** Gather some initialization common to all movable entities.
		 * @param textureId The texture to use on rendering.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param movingPixelsAmount Entity moving speed.
		 */
		MovableEntity(TextureManager::TextureId textureId, int x, int y, int movingPixelsAmount): Entity(textureId)
		{
			_x = x;
			_y = y;
			
			// Cache some parameters to fasten rendering
			_renderingDestinationRectangle.x = x;
			_renderingDestinationRectangle.y = y;
			_renderingDestinationRectangle.w = _pointerTexture->getWidth();
			_renderingDestinationRectangle.h = _pointerTexture->getHeight();
			
			_movingPixelsAmount = movingPixelsAmount;
		}
		
		/** Display the texture and rotate it in the same time. */
		virtual void render()
		{
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(&_renderingDestinationRectangle)) _pointerTexture->render(_renderingDestinationRectangle.x - Renderer::displayX, _renderingDestinationRectangle.y - Renderer::displayY, _rotationAngle);
		}
};

#endif