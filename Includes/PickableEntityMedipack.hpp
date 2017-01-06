#ifndef HPP_PICKABLE_ENTITY_MEDIPACK_HPP
#define HPP_PICKABLE_ENTITY_MEDIPACK_HPP

//#include <cstdlib>
//#include <Log.hpp>
#include <PickableEntity.hpp>
// include player
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How much life does the medipack restores. */
#define PICKABLE_ENTITY_MEDIPACK_RESTORE_LIFE_POINTS_AMOUNT 20

//-------------------------------------------------------------------------------------------------
// Class
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
			// Set the rendering texture
			/*_pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIPACK);
			if (_pointerTexture == NULL)
			{
				LOG_ERROR("Could not get medipack texture.\n");
				exit(-1);
			}*/
			
			// TODO duplicate with render rect ?
			// Set coordinates
			_x = x;
			_y = y;
			
			_renderingDestinationRectangle.x = x;
			_renderingDestinationRectangle.y = y;
			_renderingDestinationRectangle.w = _pointerTexture->getWidth();
			_renderingDestinationRectangle.h = _pointerTexture->getHeight();
		}
		
		// Nothing to clean
		~PickableEntityMedipack() {}
		
		// TODO
		int update() { return 1; }
};

#endif