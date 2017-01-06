#ifndef HPP_ENTITY_HPP
#define HPP_ENTITY_HPP

#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>
#include <TextureManager.hpp>

/** @class Entity
 * Base class for all game entity.
 */
class Entity
{
	protected:
		/** The texture used to render the entity. */
		Texture *_pointerTexture;
		
		/** Hold entity dimensions and location on the map. Use this destination rectangle to render the entity. */
		SDL_Rect _renderingDestinationRectangle;
		
	public:
		/** Load the entity texture.
		 * @param id The texture ID to use.
		 */
		Entity(TextureManager::TextureId id)
		{
			_pointerTexture = TextureManager::getTextureFromId(id);
			if (_pointerTexture == NULL)
			{
				LOG_ERROR("Could not retrieve texture.\n");
				exit(-1);
			}
		}
		
		/** Free entity allocated resources. */
		//virtual ~Entity() = 0;
		
		/** Display the entity at its current location on the map. */
		virtual void render() = 0;
		
		/** Update the internal entity logic.
		 * @return 1 if the entity must be kept alive,
		 * @return 0 if the entity must be deleted.
		 */
		virtual int update() = 0;
		
		/** Get the entity X coordinate.
		 * @return The X coordinate.
		 */
		inline int getX()
		{
			return _renderingDestinationRectangle.x;
		}
		
		/** Get the entity Y coordinate.
		 * @return The Y coordinate.
		 */
		inline int getY()
		{
			return _renderingDestinationRectangle.y;
		}
};

#endif
