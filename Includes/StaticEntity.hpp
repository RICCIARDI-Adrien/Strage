#ifndef HPP_STATIC_ENTITY_HPP
#define HPP_STATIC_ENTITY_HPP

#include <cstdlib>
#include <Entity.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <Texture.hpp>
#include <TextureManager.hpp>

/** @class StaticEntity
 * An entity that can't move on the map.
 * @author Adrien RICCIARDI
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
		 * @param textureId The texture ID to use.
		 */
		StaticEntity(int x, int y, TextureManager::TextureId textureId)
		{
			// Try to get the texture
			_pointerTexture = TextureManager::getTextureFromId(textureId);
			if (_pointerTexture == NULL)
			{
				LOG_ERROR("Could not retrieve texture (texture ID : %d).", textureId);
				exit(-1);
			}
			
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			// Cache some values to fasten processing
			_positionRectangle.w = _pointerTexture->getWidth();
			_positionRectangle.h = _pointerTexture->getHeight();
		}
		
		/** Free allocated resources. */
		virtual ~StaticEntity() {}
		
		/** Display the entity at its current location on the map. */
		virtual void render()
		{
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(&_positionRectangle)) _pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY);
		}
		
		/** Get the entity X coordinate.
		 * @return The X coordinate.
		 */
		virtual int getX()
		{
			return _positionRectangle.x;
		}
		
		/** Set the entity X coordinate.
		 * @param x The X coordinate.
		 */
		virtual void setX(int x)
		{
			_positionRectangle.x = x;
		}
		
		/** Get the entity Y coordinate.
		 * @return The Y coordinate.
		 */
		virtual int getY()
		{
			return _positionRectangle.y;
		}
		
		/** Set the entity Y coordinate.
		 * @param y The Y coordinate.
		 */
		virtual void setY(int y)
		{
			_positionRectangle.y = y;
		}
		
		/** Get a rectangle defining the entity bounds in the map.
		 * @return The position rectangle.
		 */
		virtual SDL_Rect *getPositionRectangle()
		{
			return &_positionRectangle;
		}
};

#endif