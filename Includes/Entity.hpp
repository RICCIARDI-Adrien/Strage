/** @file Entity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_ENTITY_HPP
#define HPP_ENTITY_HPP

#include <SDL2/SDL.h>

/** @class Entity
 * Base class for all game entities.
 */
class Entity
{
	public:
		/** Free allocated resources. */
		virtual ~Entity() {}
		
		/** Update the internal entity logic.
		 * @return 0 if the entity must be kept alive,
		 * @return 1 if the entity must be deleted.
		 */
		virtual int update() = 0;
		
		/** Get the entity X coordinate.
		 * @return The X coordinate.
		 */
		virtual int getX() = 0;
		
		/** Set the entity X coordinate.
		 * @param x The X coordinate.
		 */
		virtual void setX(int x) = 0;
		
		/** Get the entity Y coordinate.
		 * @return The Y coordinate.
		 */
		virtual int getY() = 0;
		
		/** Set the entity Y coordinate.
		 * @param y The Y coordinate.
		 */
		virtual void setY(int y) = 0;
		
		/** Get a rectangle defining the entity bounds in the map.
		 * @return The position rectangle.
		 */
		virtual SDL_Rect *getPositionRectangle() = 0;
};

#endif
