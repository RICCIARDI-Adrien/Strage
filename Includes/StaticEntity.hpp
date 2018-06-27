/** @file StaticEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_STATIC_ENTITY_HPP
#define HPP_STATIC_ENTITY_HPP

#include <cassert>
#include <cstdlib>
#include <Entity.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <Texture.hpp>
#include <TextureManager.hpp>

/** @class StaticEntity
 * An entity that can't move on the map.
 */
class StaticEntity: public Entity
{
	protected:
		/** The texture used to render the entity. */
		Texture *_pointerTexture;
		
		/** Hold entity dimensions and location on the map. Use this destination rectangle to render the entity. */
		SDL_Rect _positionRectangle;
		
	public:
		/** Load the entity texture.
		 * @param x Entity X coordinate.
		 * @param y Entity Y coordinate.
		 * @param pointerTexture The texture to use.
		 */
		StaticEntity(int x, int y, Texture *pointerTexture)
		{
			assert(pointerTexture != NULL);
			
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			// Cache some values to speed up processing
			_pointerTexture = pointerTexture;
			_positionRectangle.w = pointerTexture->getWidth();
			_positionRectangle.h = pointerTexture->getHeight();
		}
		
		/** Free allocated resources. */
		virtual ~StaticEntity() {}
		
		/** Display the entity at its current location on the map. */
		virtual void render()
		{
			_pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY);
		}
		
		// No need for documentation because it is the same as parent function
		virtual int getX()
		{
			return _positionRectangle.x;
		}
		
		// No need for documentation because it is the same as parent function
		virtual void setX(int x)
		{
			_positionRectangle.x = x;
		}
		
		// No need for documentation because it is the same as parent function
		virtual int getY()
		{
			return _positionRectangle.y;
		}
		
		// No need for documentation because it is the same as parent function
		virtual void setY(int y)
		{
			_positionRectangle.y = y;
		}
		
		// No need for documentation because it is the same as parent function
		virtual SDL_Rect *getPositionRectangle()
		{
			return &_positionRectangle;
		}
};

#endif
