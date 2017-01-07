#ifndef HPP_FIGHTING_ENTITY_PLAYER_HPP
#define HPP_FIGHTING_ENTITY_PLAYER_HPP

#include <cstdlib>
#include <FightingEntity.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** @class FightingEntityPlayer
 * The unique player, controller by the human player.
 * @author Adrien RICCIARDI
 */
class FightingEntityPlayer: public FightingEntity
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
		FightingEntityPlayer(int x, int y): FightingEntity(TextureManager::TEXTURE_ID_PLAYER, x, y, 3, 100)
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
		
		/** Player does not need to be updated.
		 * @return Always 0.
		 */
		virtual int update() { return 0; }
};

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The unique player, which must be accessible to any other module. */
extern FightingEntityPlayer *pointerPlayer;

#endif