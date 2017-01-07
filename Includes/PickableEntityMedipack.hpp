#ifndef HPP_PICKABLE_ENTITY_MEDIPACK_HPP
#define HPP_PICKABLE_ENTITY_MEDIPACK_HPP

#include <Log.hpp>
#include <PickableEntity.hpp>
#include <Player.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How much life does the medipack restores. */
#define PICKABLE_ENTITY_MEDIPACK_RESTORE_LIFE_POINTS_AMOUNT 20

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** @class PickableEntityMedipack
 * A medipack restoring life when player picks it.
 */
class PickableEntityMedipack: public PickableEntity
{
	public:
		/** Spawn a new medipack at the specified coordinates.
		 * @param x Vertical spawn coordinate.
		 * @param y Horizontal spawn coordinate.
		 */
		PickableEntityMedipack(int x, int y): PickableEntity(TextureManager::TEXTURE_ID_MEDIPACK)
		{
			_renderingDestinationRectangle.x = x;
			_renderingDestinationRectangle.y = y;
		}
		
		// Nothing to clean
		~PickableEntityMedipack() {}
		
		// Check if the medipack can be taken by the player
		int update()
		{
			// Nothing to do if the medipack is not visible on the display (i.e. the player can't grab it)
			if (!Renderer::isDisplayable(&_renderingDestinationRectangle)) return 0;
			
			// Is the player colliding with the medipack ?
			if (!SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_renderingDestinationRectangle)) return 0;
			LOG_DEBUG("Player is colliding with medipack.\n");
			
			// The player is colliding, can he be healed ?
			// TODO, return 1 if taken
			
			return 1;
		}
};

#endif