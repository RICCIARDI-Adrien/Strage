#ifndef HPP_PICKABLE_ENTITY_MEDIPACK_HPP
#define HPP_PICKABLE_ENTITY_MEDIPACK_HPP

#include <FightingEntityPlayer.hpp>
#include <Log.hpp>
#include <PickableEntity.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** @class PickableEntityMedipack
 * A medipack restoring life when player picks it.
 * @author Adrien RICCIARDI
 */
class PickableEntityMedipack: public PickableEntity
{
	public:
		/** Spawn a new medipack at the specified coordinates.
		 * @param x Vertical spawn coordinate.
		 * @param y Horizontal spawn coordinate.
		 */
		PickableEntityMedipack(int x, int y): PickableEntity(TextureManager::TEXTURE_ID_MEDIPACK, x, y) {}
		
		/** Free entity allocated resources. */
		virtual ~PickableEntityMedipack() {} // Nothing to free
		
		/** Check if the medipack can be taken by the player.
		 * @return 0 if the medipack must remain on the map,
		 * @return 1 if the medipack must be removed from the map.
		 */
		int update()
		{
			// Nothing to do if the medipack is not visible on the display (i.e. the player can't grab it)
			if (!Renderer::isDisplayable(&_positionRectangle)) return 0;
			
			// Is the player colliding with the medipack ?
			if (!SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_positionRectangle)) return 0;
			LOG_DEBUG("Player is colliding with medipack.\n");
			
			// The player is colliding, can he be healed ?
			if (pointerPlayer->isWounded())
			{
				pointerPlayer->modifyLife(20);
				LOG_DEBUG("Healed player.\n");
				return 1; // Remove the medipack as it has been used
			}
			else return 0; // Player is healthy, the medipack has not been used, so don't remove it
		}
};

#endif