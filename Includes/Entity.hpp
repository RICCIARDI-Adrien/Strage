#ifndef HPP_ENTITY_HPP
#define HPP_ENTITY_HPP

#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>

/** @class Entity
 * Base class for all game entity.
 */
class Entity
{
	protected:
		/** The texture used to render the entity. */
		Texture *_pointerTexture;
		
		// TODO Useful with rectangle ?
		/** The entity left coordinate on the map. */
		int _x;
		/** The entity top coordinate on the map. */
		int _y;
		
		/** Use this destination rectangle (with x and y parameters updated) to render the entity. */
		SDL_Rect _renderingDestinationRectangle;
		
		/** Tell if the entity can be displayed or not (it is out of the display area).
		 * @return 1 if the entity can be displayed,
		 * @return 0 if not.
		 */
		/*int isDisplayable()
		{
			
		}*/
		
	public:
		//Entity() {}
		
		/** Free entity allocated resources. */
		//virtual ~Entity() = 0;
		
		/** Display the entity at its current location on the map. */
		virtual void render() = 0;
		
		/** Update the internal entity logic.
		 * @return 1 if the entity must be kept alive,
		 * @return 0 if the entity must be deleted.
		 */
		virtual int update() = 0;
};

#endif
