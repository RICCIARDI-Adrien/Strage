#ifndef HPP_PICKABLE_ENTITY_AMMUNITION_HPP
#define HPP_PICKABLE_ENTITY_AMMUNITION_HPP

#include <FightingEntityPlayer.hpp>
#include <Log.hpp>
#include <PickableEntity.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** @class PickableEntityAmmunition
 * Give player more ammunition.
 * @author Adrien RICCIARDI
 */
class PickableEntityAmmunition: public PickableEntity
{
	public:
		/** Spawn new ammunition at the specified coordinates.
		 * @param x Vertical spawn coordinate.
		 * @param y Horizontal spawn coordinate.
		 */
		PickableEntityAmmunition(int x, int y): PickableEntity(TextureManager::TEXTURE_ID_AMMUNITION) // TODO put x and y in Entity
		{
			_positionRectangle.x = x;
			_positionRectangle.y = y;
		}
		
		/** Free entity allocated resources. */
		virtual ~PickableEntityAmmunition() {} // Nothing to free
		
		/** Check if ammunition can be taken by the player.
		 * @return 0 if ammunition must remain on the map,
		 * @return 1 if ammunition must be removed from the map.
		 */
		int update()
		{
			// Nothing to do if ammunition are not visible on the display (i.e. the player can't grab them)
			if (!Renderer::isDisplayable(&_positionRectangle)) return 0;
			
			// Is the player colliding with ammunition ?
			if (!SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_positionRectangle)) return 0;
			
			// Increase player ammunition
			pointerPlayer->addAmmunition(30);
			LOG_DEBUG("Player got ammunition.\n");
			
			return 1;
		}
};

#endif