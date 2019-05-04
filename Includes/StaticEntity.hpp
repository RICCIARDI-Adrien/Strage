/** @file StaticEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_STATIC_ENTITY_HPP
#define HPP_STATIC_ENTITY_HPP

#include <Entity.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>

/** @class StaticEntity
 * An entity that can't move on the map.
 */
class StaticEntity: public Entity
{
	public:
		/** Load the entity texture.
		 * @param x Entity X coordinate.
		 * @param y Entity Y coordinate.
		 * @param pointerTexture The texture to use.
		 */
		StaticEntity(int x, int y, Texture *pointerTexture);
		
		// No need for documentation because it is the same as parent function
		virtual ~StaticEntity() {}
		
		/** Display the entity at its current location on the map. */
		virtual void render();
		
		// No need for documentation because it is the same as parent function
		virtual int getX();
		
		// No need for documentation because it is the same as parent function
		virtual void setX(int x);
		
		// No need for documentation because it is the same as parent function
		virtual int getY();
		
		// No need for documentation because it is the same as parent function
		virtual void setY(int y);
		
		// No need for documentation because it is the same as parent function
		virtual SDL_Rect *getPositionRectangle();

	protected:
		/** The texture used to render the entity. */
		Texture *_pointerTexture;
		
		/** Hold entity dimensions and location on the map. Use this destination rectangle to render the entity. */
		SDL_Rect _positionRectangle;
};

#endif
