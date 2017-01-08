#ifndef HPP_PICKABLE_ENTITY_HPP
#define HPP_PICKABLE_ENTITY_HPP

#include <Entity.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class PickableEntity
 * A still entity that can be picked up by the player by walking across the entity.
 * @author Adrien RICCIARDI
 */
class PickableEntity: public Entity
{
	protected:
		/** Tell if the player entered the picking area of the entity.
		 * @return 0 if the entity is not colliding with the player,
		 * @return 1 if the entity is colliding with the player.
		 */
		int isCollidingWithPlayer();
	
	public:
		/** Call super class constructor to load the texture.
		 * @param textureId The texture to use when rendering the entity.
		 */
		PickableEntity(TextureManager::TextureId textureId): Entity(textureId) {}
		
		/** Free entity resources. */
		//virtual ~PickableEntity();
		
		/** @see Entity for description. */
		void render()
		{
			// Display the texture only if the entity is visible on screen
			if (Renderer::isDisplayable(&_positionRectangle)) _pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY);
		}
	
		/** @see Entity for description. */
		virtual int update() = 0;
};

#endif