#ifndef HPP_FIGHTING_ENTITY_PLAYER_HPP
#define HPP_FIGHTING_ENTITY_PLAYER_HPP

#include <AudioManager.hpp>
#include <FightingEntity.hpp>
#include <Log.hpp>
#include <MovingEntityBullet.hpp>
#include <MovingEntityBulletPlayer.hpp>
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
	
	protected:
		/** Shoot a player ammunition.
		 * @param x Bullet X coordinate on the map.
		 * @param y Bullet Y coordinate on the map.
		 */
		virtual MovingEntityBullet *_fireBullet(int x, int y)
		{
			return new MovingEntityBulletPlayer(x, y, _facingDirection);
		}
	
	public:
		/** Create the player.
		 * @param x X coordinate where to spawn the player on the map.
		 * @param y Y coordinate where to spawn the player on the map.
		 */
		FightingEntityPlayer(int x, int y): FightingEntity(x, y, TextureManager::TEXTURE_ID_PLAYER, 3, 100, 300, AudioManager::SOUND_ID_PLAYER_FIRESHOT)
		{
			// Cache rendering coordinates
			_renderingX = (Renderer::displayWidth - _positionRectangle.w) / 2;
			_renderingY = (Renderer::displayHeight - _positionRectangle.h) / 2;
			
			_ammunitionAmount = 100;
		}
		
		/** Free allocated resources. */
		virtual ~FightingEntityPlayer() {}
		
		/** @see FightingEntity for description. */
		virtual MovingEntityBullet *shoot()
		{
			MovingEntityBullet *pointerBullet;
			
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
		
		/** Set the remaining ammunition count.
		 * @param ammunitionCount The ammunition amount.
		 */
		inline void setAmmunitionAmount(int ammunitionCount)
		{
			_ammunitionAmount = ammunitionCount;
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
		
		/** Check for a pickable item on the underlaying block and get it if possible.
		 * @return 0 if the game should continue,
		 * @return 2 if the next level must be loaded.
		 */
		virtual int update()
		{
			int blockContent, playerCenterX, playerCenterY;
			
			// Cache player center coordinates
			playerCenterX = _positionRectangle.x + (_positionRectangle.w / 2);
			playerCenterY = _positionRectangle.y + (_positionRectangle.h / 2);
			
			// Get block under player center content
			blockContent = LevelManager::getBlockContent(playerCenterX, playerCenterY);
				
			// Is there a medipack ?
			if (blockContent & LevelManager::BLOCK_CONTENT_MEDIPACK)
			{
				LOG_DEBUG("Player is crossing a block containing a medipack.");
				
				if (isWounded())
				{
					modifyLife(20);
					AudioManager::playSound(AudioManager::SOUND_ID_PLAYER_HEALED);
					LOG_DEBUG("Healed player.");
					
					// Remove the medipack as it has been used
					blockContent &= ~LevelManager::BLOCK_CONTENT_MEDIPACK;
					LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
				}
			}
			// Is there a golden medipack ?
			if (blockContent & LevelManager::BLOCK_CONTENT_GOLDEN_MEDIPACK)
			{
				LOG_DEBUG("Player is crossing a block containing a golden medipack.");
				
				// Increase maximum life
				_maximumLifePointsAmount += 10;
				// Restore life
				_lifePointsAmount = _maximumLifePointsAmount;
				//AudioManager::playSound(AudioManager::SOUND_ID_PLAYER_HEALED); TODO
				LOG_DEBUG("Increased player life and healed him.");
				
				// Remove the golden medipack as it has been used
				blockContent &= ~LevelManager::BLOCK_CONTENT_GOLDEN_MEDIPACK;
				LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
			}
			// Is there ammunition ?
			else if (blockContent & LevelManager::BLOCK_CONTENT_AMMUNITION)
			{
				LOG_DEBUG("Player is crossing a block containing ammunition.");
				
				addAmmunition(30);
				AudioManager::playSound(AudioManager::SOUND_ID_AMMUNITION_TAKEN);
				LOG_DEBUG("Player got ammunition.");
				
				// Remove the ammunition item
				blockContent &= ~LevelManager::BLOCK_CONTENT_AMMUNITION;
				LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
			}
			// Is it the level end ?
			else if (blockContent & LevelManager::BLOCK_CONTENT_LEVEL_EXIT) return 2;
			
			return 0;
		}
};

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The unique player, which must be accessible to any other module. */
extern FightingEntityPlayer *pointerPlayer;

#endif