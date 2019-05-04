/** @file MovingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MOVING_ENTITY_HPP
#define HPP_MOVING_ENTITY_HPP

#include <Entity.hpp>
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
		
		/** Gather some initialization common to all moving entities.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param facingUpTextureId The texture to use on rendering when the entity is facing up. This texture ID must be followed by facing down, facing left and facing right textures (in this order).
		 * @param movingPixelsAmount Entity moving speed.
		 */
		MovingEntity(int x, int y, TextureManager::TextureId facingUpTextureId, int movingPixelsAmount);
		
		/** Free allocated resources. */
		virtual ~MovingEntity();
		
		/** Get the entity X coordinate.
		 * @return The X coordinate.
		 */
		virtual int getX();
		
		/** Set the entity X coordinate.
		 * @param x The X coordinate.
		 */
		virtual void setX(int x);
		
		/** Get the entity Y coordinate.
		 * @return The Y coordinate.
		 */
		virtual int getY();
		
		/** Set the entity Y coordinate.
		 * @param y The Y coordinate.
		 */
		virtual void setY(int y);
		
		/** Get a rectangle defining the entity bounds in the map.
		 * @return The position rectangle.
		 */
		virtual SDL_Rect *getPositionRectangle();
		
		/** Move the entity to the up.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToUp();
		
		/** Move the entity to the down.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToDown();
		
		/** Move the entity to the left.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToLeft();
		
		/** Move the entity to the right.
		 * @return How many pixels the entity moved.
		 */
		virtual int moveToRight();
		
		/** Make the entity move at its speed to the specified direction.
		 * @param movingDirection The direction to move to.
		 * @return How many pixels the entity moved.
		 */
		virtual int move(Direction movingDirection);
		
		/** Turn the entity to the up direction. */
		void turnToUp();
		
		/** Turn the entity to the down direction. */
		void turnToDown();
		
		/** Turn the entity to the left direction. */
		void turnToLeft();
		
		/** Turn the entity to the right direction. */
		void turnToRight();
		
		/** Display the texture facing in the right direction. */
		virtual void render();
		
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
};

#endif
