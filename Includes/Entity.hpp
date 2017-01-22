#ifndef HPP_ENTITY_HPP
#define HPP_ENTITY_HPP

#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>
#include <TextureManager.hpp>

/** @class Entity
 * Base class for all game entities.
 * @author Adrien RICCIARDI
 */
class Entity
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
		Entity(int x, int y, TextureManager::TextureId textureId)
		{
			// Try to get the texture
			_pointerTexture = TextureManager::getTextureFromId(textureId);
			if (_pointerTexture == NULL)
			{
				LOG_ERROR("Could not retrieve texture.");
				exit(-1);
			}
			
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			// Cache some values to fasten processing
			_positionRectangle.w = _pointerTexture->getWidth();
			_positionRectangle.h = _pointerTexture->getHeight();
		}
		
		/** Display the entity at its current location on the map. */
		virtual void render()
		{
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(&_positionRectangle)) _pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY);
		}
		
		/** Update the internal entity logic.
		 * @return 0 if the entity must be kept alive,
		 * @return 1 if the entity must be deleted.
		 */
		virtual int update() = 0;
		
		/** Get the entity X coordinate.
		 * @return The X coordinate.
		 */
		inline int getX()
		{
			return _positionRectangle.x;
		}
		
		/** Set the entity X coordinate.
		 * @param x The X coordinate.
		 */
		inline void setX(int x)
		{
			_positionRectangle.x = x;
		}
		
		/** Set the entity Y coordinate.
		 * @param y The Y coordinate.
		 */
		inline void setY(int y)
		{
			_positionRectangle.y = y;
		}
		
		/** Get the entity Y coordinate.
		 * @return The Y coordinate.
		 */
		inline int getY()
		{
			return _positionRectangle.y;
		}
		
		/** Get a rectangle defining the entity bounds in the map.
		 * @return The position rectangle.
		 */
		inline SDL_Rect *getPositionRectangle()
		{
			return &_positionRectangle;
		}
};

#endif
