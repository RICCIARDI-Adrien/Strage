#ifndef HPP_FIGHTING_ENTITY_HPP
#define HPP_FIGHTING_ENTITY_HPP

#include <MovableEntity.hpp>
#include <MovableEntityBullet.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntity
 * A movable entity that has ammunitions and life points.
 * @author Adrien RICCIARDI
 */
class FightingEntity: public MovableEntity
{
	protected:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** How many ammunitions the entity owns. */
		int _ammunitionsAmount;
		
		/** Add this offset to the entity coordinates to make the fired bullet start from the entity center. */
		int _bulletStartingPositionOffset;
		
		/** When was the last shot fired (in milliseconds). This is part of the fire rate mechanism. */
		unsigned int _lastShotTime;
		/** How many milliseconds to wait between two shots. */
		unsigned int _timeBetweenShots;
		
	public:
		/** Initialize life points in addition to parent classes fields.
		 * @param textureId The texture to use on rendering.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param movingPixelsAmount Entity moving speed.
		 * @param maximumLifePointsAmount Entity maximum life points count.
		 */
		FightingEntity(TextureManager::TextureId textureId, int x, int y, int movingPixelsAmount, int maximumLifePointsAmount): MovableEntity(textureId, x, y, movingPixelsAmount) // TODO _timeBetweenShots
		{
			_lifePointsAmount = maximumLifePointsAmount;
			_maximumLifePointsAmount = maximumLifePointsAmount;
			
			// Cache the offset to add to entity coordinates to make fired bullets start from entity center
			Texture *pointerBulletTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BULLET); // Use textures to avoid instantiate a bullet to get its dimensions
			_bulletStartingPositionOffset = (_pointerTexture->getWidth() - pointerBulletTexture->getWidth()) / 2; // This works in all fighting entity facing directions because fighting entity textures are appositely circular
			_lastShotTime = 0; // Allow to shoot immediately
		}
		
		/** The the entity life points.
		 * @return Entity life point amount.
		 */
		inline int getLifePointsAmount()
		{
			return _lifePointsAmount;
		}
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 */
		inline void modifyLife(int lifePointsAmount)
		{
			_lifePointsAmount += lifePointsAmount;
			
			// Clamp life points to [0; _maximumLifePointsAmount]
			if (_lifePointsAmount < 0) _lifePointsAmount = 0;
			else if (_lifePointsAmount > _maximumLifePointsAmount) _lifePointsAmount = _maximumLifePointsAmount;
		}
		
		/** Tell if the entity is at maximum life or not.
		 * @return 1 if the entity can be healed,
		 * @return 0 if the entity is healthy.
		 */
		inline int isWounded()
		{
			if (_lifePointsAmount < _maximumLifePointsAmount) return 1;
			return 0;
		}
		
		/** Generate a bullet facing the entity direction.
		 * @return A valid pointer if the entity was allowed to shot,
		 * @return NULL if the entity could not shoot (no more ammunitions, slower fire rate...).
		 */
		MovableEntityBullet *shoot()
		{
			// Allow to shoot only if enough time elapsed since last shot
			if (SDL_GetTicks() - _lastShotTime >= _timeBetweenShots)
			{
				MovableEntityBullet *pointerBullet = new MovableEntityBullet(_renderingDestinationRectangle.x + _bulletStartingPositionOffset, _renderingDestinationRectangle.y + _bulletStartingPositionOffset, _facingDirection);
				
				// Get time after having generated the bullet, in case this takes more than 1 millisecond
				_lastShotTime = SDL_GetTicks();
				
				return pointerBullet;
			}
			else return NULL; // No shot allowed
		}
};

#endif