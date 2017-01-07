#ifndef HPP_PLAYER_HPP
#define HPP_PLAYER_HPP

#include <cstdlib>
#include <Log.hpp>
#include <MovableEntity.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

// TODO rename to xxxEntityPlayer
//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
class Player: public MovableEntity // TODO add additionnal class for movable objects with life points
{
	private:
		/** Display the player at this horizontal display coordinate. */
		int _renderingX;
		/** Display the player at this vertical display coordinate. */
		int _renderingY;
	
	public:
		/** Create the player.
		 * @param x X coordinate where to spawn the player on the map.
		 * @param y Y coordinate where to spawn the player on the map.
		 */
		Player(int x, int y): MovableEntity(TextureManager::TEXTURE_ID_PLAYER, x, y, 3)
		{
			// Cache rendering coordinates
			_renderingX = (CONFIGURATION_DISPLAY_WIDTH - _renderingDestinationRectangle.w) / 2;
			_renderingY = (CONFIGURATION_DISPLAY_HEIGHT - _renderingDestinationRectangle.h) / 2;
		}
		
		/** Display the player at the screen center. */
		virtual void render()
		{
			// Display the player at the screen center
			_pointerTexture->render(_renderingX, _renderingY, _rotationAngle);
		}
		
		virtual int update() { return 1; }
};

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The unique player, which must be accessible to any other module. */
extern Player *pointerPlayer;

#endif