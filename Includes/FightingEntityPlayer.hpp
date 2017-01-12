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
		
		/** Check for a pickable item on the underlaying block and get it if possible. */
		void _handlePickableItem()
		{
			int blockContent;
			
			// Get block under player center content
			blockContent = LevelManager::getBlockContent(_positionRectangle.x + (_positionRectangle.w / 2), _positionRectangle.y + (_positionRectangle.h / 2));
				
			// Is there a medipack ?
			if (blockContent & LevelManager::BLOCK_CONTENT_MEDIPACK)
			{
				LOG_DEBUG("Player is crossing a block containing a medipack.\n");
				
				if (isWounded())
				{
					modifyLife(20);
					LOG_DEBUG("Healed player.\n");
					
					// Remove the medipack as it has been used
					blockContent &= ~LevelManager::BLOCK_CONTENT_MEDIPACK;
					LevelManager::setBlockContent(_positionRectangle.x + (_positionRectangle.w / 2), _positionRectangle.y + (_positionRectangle.h / 2), blockContent);
				}
			}
			// Is there ammunition ?
			else if (blockContent & LevelManager::BLOCK_CONTENT_AMMUNITION)
			{
				LOG_DEBUG("Player is crossing a block containing ammunition.\n");
				
				addAmmunition(30);
				LOG_DEBUG("Player got ammunition.\n");
				
				// Remove the ammunitions item
				blockContent &= ~LevelManager::BLOCK_CONTENT_AMMUNITION;
				LevelManager::setBlockContent(_positionRectangle.x + (_positionRectangle.w / 2), _positionRectangle.y + (_positionRectangle.h / 2), blockContent);
			}
		}
	
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
		
		/** Move the player to the up and check for a pickable item on the new location.
		 * @return How many pixels the player moved.
		 */
		virtual int moveToUp()
		{
			int movedPixelsAmount;
			
			movedPixelsAmount = MovableEntity::moveToUp();
			
			// If the player really moved, check for a pickable item on the new location
			if (movedPixelsAmount) _handlePickableItem();
			
			return movedPixelsAmount;
		}
		
		/** Move the player to the down and check for a pickable item on the new location.
		 * @return How many pixels the player moved.
		 */
		virtual int moveToDown()
		{
			int movedPixelsAmount;
			
			movedPixelsAmount = MovableEntity::moveToDown();
			
			// If the player really moved, check for a pickable item on the new location
			if (movedPixelsAmount) _handlePickableItem();
			
			return movedPixelsAmount;
		}
		
		/** Move the player to the left and check for a pickable item on the new location.
		 * @return How many pixels the player moved.
		 */
		virtual int moveToLeft()
		{
			int movedPixelsAmount;
			
			movedPixelsAmount = MovableEntity::moveToLeft();
			
			// If the player really moved, check for a pickable item on the new location
			if (movedPixelsAmount) _handlePickableItem();
			
			return movedPixelsAmount;
		}
		
		/** Move the player to the right and check for a pickable item on the new location.
		 * @return How many pixels the player moved.
		 */
		virtual int moveToRight()
		{
			int movedPixelsAmount;
			
			movedPixelsAmount = MovableEntity::moveToRight();
			
			// If the player really moved, check for a pickable item on the new location
			if (movedPixelsAmount) _handlePickableItem();
			
			return movedPixelsAmount;
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