#ifndef HPP_FIGHTING_ENTITY_PLAYER_HPP
#define HPP_FIGHTING_ENTITY_PLAYER_HPP

#include <cstdlib>
#include <FightingEntity.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** @class FightingEntityPlayer
 * The unique player, controlled by the human player.
 * @author Adrien RICCIARDI
 */
class FightingEntityPlayer: public FightingEntity
{
	private:
		/** Display the player at this horizontal display coordinate. */
		int _renderingX;
		/** Display the player at this vertical display coordinate. */
		int _renderingY;
		
		/** How many ammunition the player owns. */
		int _ammunitionAmount; // Only player has limited ammunition, because what could do a munitions-less enemy ?
	
	public:
		/** Create the player.
		 * @param x X coordinate where to spawn the player on the map.
		 * @param y Y coordinate where to spawn the player on the map.
		 */
		FightingEntityPlayer(int x, int y): FightingEntity(TextureManager::TEXTURE_ID_PLAYER, x, y, 3, 100, 300)
		{
			// Cache rendering coordinates
			_renderingX = (CONFIGURATION_DISPLAY_WIDTH - _positionRectangle.w) / 2;
			_renderingY = (CONFIGURATION_DISPLAY_HEIGHT - _positionRectangle.h) / 2;
			
			_ammunitionAmount = 100;
		}
		
		/** Free entity allocated resources. */
		virtual ~FightingEntityPlayer() {} // Nothing to free
		
		/** @see FightingEntity for description. */
		virtual MovableEntityBullet *shoot()
		{
			MovableEntityBullet *pointerBullet;
			
			// The player can't shoot if it has no more ammunition
			if (_ammunitionAmount == 0) return NULL;
			
			// Decrement ammunition count if the player shot
			pointerBullet = FightingEntity::shoot();
			if (pointerBullet != NULL) _ammunitionAmount--;
			
			return pointerBullet;
		}
		
		/** Get the remaining ammunition count.
		 * @return The ammunition amount.
		 */
		inline int getAmmunitionAmount()
		{
			return _ammunitionAmount;
		}
		
		/** Add ammunition to the player owned ones.
		 * @param amount How many ammunition to add (do not use negative numbers, there is no check...).
		 */
		inline void addAmmunition(int amount)
		{
			_ammunitionAmount += amount;
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