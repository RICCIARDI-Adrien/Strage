/** @file MovingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MOVING_ENTITY_HPP
#define HPP_MOVING_ENTITY_HPP

#include <Entity.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class MovingEntity
 * An entity that can move up, down, left and right onto the map. A moving entity can't walk across a scene wall.
 */
class MovingEntity: public Entity
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
		/** Tell in which direction the entity is facing. */
		Direction _facingDirection;
		
		/** How many pixels to move the entity. */
		int _movingPixelsAmount;
		
		/** The block content bit mask to search against for collision. */
		int _collisionBlockContent;
		
		/** All entity textures (one for each direction). */
		Texture *_pointerTextures[DIRECTIONS_COUNT];
		/** All entity collision rectangles (one for each direction). */
		SDL_Rect _positionRectangles[DIRECTIONS_COUNT];
	
	public:
		/** Gather some initialization common to all moving entities.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param facingUpTextureId The texture to use on rendering when the entity is facing up. This texture ID must be followed by facing down, facing left and facing right textures (in this order).
		 * @param movingPixelsAmount Entity moving speed.
		 */
		MovingEntity(int x, int y, TextureManager::TextureId facingUpTextureId, int movingPixelsAmount)
		{
			int i, textureId = (int) facingUpTextureId;
			
			// Set all textures and collision rectangles
			for (i = 0; i < DIRECTIONS_COUNT; i++)
			{
				// Set the texture facing in the right direction
				_pointerTextures[i] = TextureManager::getTextureFromId((TextureManager::TextureId) textureId);
				textureId++;
				
				// Set collision rectangle
				_positionRectangles[i].x = x;
				_positionRectangles[i].y = y;
				_positionRectangles[i].h = _pointerTextures[i]->getHeight();
				_positionRectangles[i].w = _pointerTextures[i]->getWidth();
			}
			
			_movingPixelsAmount = movingPixelsAmount;
			
			// Collide with walls by default
			_collisionBlockContent = LevelManager::BLOCK_CONTENT_WALL;
			
			// Entity is facing up on spawn
			_facingDirection = DIRECTION_UP;
		}
		
		/** Free allocated resources. */
		virtual ~MovingEntity() {}
		
		/** Get the entity X coordinate.
		 * @return The X coordinate.
		 */
		virtual int getX()
		{
			return _positionRectangles[_facingDirection].x;
		}
		
		/** Set the entity X coordinate.
		 * @param x The X coordinate.
		 */
		virtual void setX(int x)
		{
			// Set all rectangles X coordinate (do not use a loop to fasten the code)
			_positionRectangles[DIRECTION_UP].x = x;
			_positionRectangles[DIRECTION_DOWN].x = x;
			_positionRectangles[DIRECTION_LEFT].x = x;
			_positionRectangles[DIRECTION_RIGHT].x = x;
		}
		
		/** Get the entity Y coordinate.
		 * @return The Y coordinate.
		 */
		virtual int getY()
		{
			return _positionRectangles[_facingDirection].y;
		}
		
		/** Set the entity Y coordinate.
		 * @param y The Y coordinate.
		 */
		virtual void setY(int y)
		{
			// Set all rectangles Y coordinate (do not use a loop to fasten the code)
			_positionRectangles[DIRECTION_UP].y = y;
			_positionRectangles[DIRECTION_DOWN].y = y;
			_positionRectangles[DIRECTION_LEFT].y = y;
			_positionRectangles[DIRECTION_RIGHT].y = y;
		}
		
		/** Get a rectangle defining the entity bounds in the map.
		 * @return The position rectangle.
		 */
		virtual SDL_Rect *getPositionRectangle()
		{
			return &_positionRectangles[_facingDirection];
		}
		
		/** Move the entity to the up.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToUp()
		{
			int leftSideDistanceToWall, rightSizeDistanceToWall, distanceToWall, movingPixelsAmount;
			SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
			
			// Check upper border distance against level walls
			leftSideDistanceToWall = LevelManager::getDistanceFromUpperBlock(pointerPositionRectangle->x, pointerPositionRectangle->y, _collisionBlockContent);
			rightSizeDistanceToWall = LevelManager::getDistanceFromUpperBlock(pointerPositionRectangle->x + pointerPositionRectangle->w - 1, pointerPositionRectangle->y, _collisionBlockContent);
			
			// Keep the smaller distance
			if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
			else distanceToWall = rightSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			// Update all rectangles
			_positionRectangles[DIRECTION_UP].y -= movingPixelsAmount;
			_positionRectangles[DIRECTION_DOWN].y -= movingPixelsAmount;
			_positionRectangles[DIRECTION_LEFT].y -= movingPixelsAmount;
			_positionRectangles[DIRECTION_RIGHT].y -= movingPixelsAmount;
			
			// Entity is facing up
			_facingDirection = DIRECTION_UP;
			
			return movingPixelsAmount;
		}
		
		/** Move the entity to the down.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToDown()
		{
			int leftSideDistanceToWall, rightSizeDistanceToWall, distanceToWall, movingPixelsAmount;
			SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
			
			// Check downer border distance against level walls
			leftSideDistanceToWall = LevelManager::getDistanceFromDownerBlock(pointerPositionRectangle->x, pointerPositionRectangle->y + pointerPositionRectangle->h, _collisionBlockContent);
			rightSizeDistanceToWall = LevelManager::getDistanceFromDownerBlock(pointerPositionRectangle->x + pointerPositionRectangle->w - 1, pointerPositionRectangle->y + pointerPositionRectangle->h, _collisionBlockContent);
			
			// Keep the smaller distance
			if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
			else distanceToWall = rightSizeDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			// Update all rectangles
			_positionRectangles[DIRECTION_UP].y += movingPixelsAmount;
			_positionRectangles[DIRECTION_DOWN].y += movingPixelsAmount;
			_positionRectangles[DIRECTION_LEFT].y += movingPixelsAmount;
			_positionRectangles[DIRECTION_RIGHT].y += movingPixelsAmount;
			
			// Entity is facing down
			_facingDirection = DIRECTION_DOWN;
			
			return movingPixelsAmount;
		}
		
		/** Move the entity to the left.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToLeft()
		{
			int upperSideDistanceToWall, downerSideDistanceToWall, distanceToWall, movingPixelsAmount;
			SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
			
			// Check leftmost border distance against level walls
			upperSideDistanceToWall = LevelManager::getDistanceFromLeftmostBlock(pointerPositionRectangle->x, pointerPositionRectangle->y, _collisionBlockContent);
			downerSideDistanceToWall = LevelManager::getDistanceFromLeftmostBlock(pointerPositionRectangle->x, pointerPositionRectangle->y + pointerPositionRectangle->h - 1, _collisionBlockContent);
			
			// Keep the smaller distance
			if (upperSideDistanceToWall < downerSideDistanceToWall) distanceToWall = upperSideDistanceToWall;
			else distanceToWall = downerSideDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			// Update all rectangles
			_positionRectangles[DIRECTION_UP].x -= movingPixelsAmount;
			_positionRectangles[DIRECTION_DOWN].x -= movingPixelsAmount;
			_positionRectangles[DIRECTION_LEFT].x -= movingPixelsAmount;
			_positionRectangles[DIRECTION_RIGHT].x -= movingPixelsAmount;
			
			// Entity is facing left
			_facingDirection = DIRECTION_LEFT;
			
			return movingPixelsAmount;
		}
		
		/** Move the entity to the right.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToRight()
		{
			int upperSideDistanceToWall, downerSideDistanceToWall, distanceToWall, movingPixelsAmount;
			SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
			
			// Check rightmost border distance against level walls
			upperSideDistanceToWall = LevelManager::getDistanceFromRightmostBlock(pointerPositionRectangle->x + pointerPositionRectangle->w, pointerPositionRectangle->y, _collisionBlockContent);
			downerSideDistanceToWall = LevelManager::getDistanceFromRightmostBlock(pointerPositionRectangle->x + pointerPositionRectangle->w, pointerPositionRectangle->y + pointerPositionRectangle->h - 1, _collisionBlockContent);
			
			// Keep the smaller distance
			if (upperSideDistanceToWall < downerSideDistanceToWall) distanceToWall = upperSideDistanceToWall;
			else distanceToWall = downerSideDistanceToWall;
			
			// Move if possible
			if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
			else movingPixelsAmount = distanceToWall;
			// Update all rectangles
			_positionRectangles[DIRECTION_UP].x += movingPixelsAmount;
			_positionRectangles[DIRECTION_DOWN].x += movingPixelsAmount;
			_positionRectangles[DIRECTION_LEFT].x += movingPixelsAmount;
			_positionRectangles[DIRECTION_RIGHT].x += movingPixelsAmount;
			
			// Entity is facing right
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
			_facingDirection = DIRECTION_UP;
		}
		
		/** Turn the entity to the down direction. */
		void turnToDown()
		{
			_facingDirection = DIRECTION_DOWN;
		}
		
		/** Turn the entity to the left direction. */
		void turnToLeft()
		{
			_facingDirection = DIRECTION_LEFT;
		}
		
		/** Turn the entity to the right direction. */
		void turnToRight()
		{
			_facingDirection = DIRECTION_RIGHT;
		}
		
		/** Display the texture facing in the right direction. */
		virtual void render()
		{
			SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
			
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(pointerPositionRectangle)) _pointerTextures[_facingDirection]->render(pointerPositionRectangle->x - Renderer::displayX, pointerPositionRectangle->y - Renderer::displayY);
			
			// Display collision rectangle in debug mode
			#if CONFIGURATION_LOG_LEVEL == 3
			{
				SDL_Rect renderingRectangle, *pointerPositionRectangle;
				
				// Position rectangle
				SDL_SetRenderDrawColor(Renderer::pointerRenderer, 0, 255, 255, 255);
				
				pointerPositionRectangle = &_positionRectangles[_facingDirection];
				renderingRectangle.x = pointerPositionRectangle->x - Renderer::displayX;
				renderingRectangle.y = pointerPositionRectangle->y - Renderer::displayY;
				renderingRectangle.w = pointerPositionRectangle->w;
				renderingRectangle.h = pointerPositionRectangle->h;
				SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
				
				// Restore background color
				SDL_SetRenderDrawColor(Renderer::pointerRenderer, 0, 0, 0, 255);
			}
			#endif
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
