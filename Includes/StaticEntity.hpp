/** @file StaticEntity.hpp
 * @author Adrien RICCIARDI
 */
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
