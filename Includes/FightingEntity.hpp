#ifndef HPP_FIGHTING_ENTITY_HPP
#define HPP_FIGHTING_ENTITY_HPP

#include <AudioManager.hpp>
#include <MovingEntity.hpp>
#include <MovingEntityBullet.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntity
 * A moving entity that has life points and can shoot bullets.
 * @author Adrien RICCIARDI
 */
class FightingEntity: public MovingEntity
{
	protected:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** Offset to add to entity X coordinate to fire the bullet to up direction. */
		int _bulletStartingPositionOffsetUpX;
		/** Offset to add to entity Y coordinate to fire the bullet to up direction. */
		int _bulletStartingPositionOffsetUpY;
		/** Offset to add to entity X coordinate to fire the bullet to down direction. */
		int _bulletStartingPositionOffsetDownX;
		/** Offset to add to entity Y coordinate to fire the bullet to down direction. */
		int _bulletStartingPositionOffsetDownY;
		/** Offset to add to entity X coordinate to fire the bullet to left direction. */
		int _bulletStartingPositionOffsetLeftX;
		/** Offset to add to entity Y coordinate to fire the bullet to left direction. */
		int _bulletStartingPositionOffsetLeftY;
		/** Offset to add to entity X coordinate to fire the bullet to right direction. */
		int _bulletStartingPositionOffsetRightX;
		/** Offset to add to entity Y coordinate to fire the bullet to right direction. */
		int _bulletStartingPositionOffsetRightY;
		
		/** When was the last shot fired (in milliseconds). This is part of the fire rate mechanism. */
		unsigned int _lastShotTime;
		/** How many milliseconds to wait between two shots. */
		unsigned int _timeBetweenShots;
		
		/** The sound to play when the entity shoots. */
		AudioManager::SoundId _firingSoundId;
		
		/** Generate a bullet from the type the enemy fires.
		 * @param x Bullet X coordinate.
		 * @param y Bullet Y coordinate.
		 * @return An enemy-specific bullet.
		 */
		virtual MovingEntityBullet *_fireBullet(int x, int y) = 0;
		
	public:
		/** Initialize life points in addition to parent classes fields.
		 * @param textureId The texture to use on rendering.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param movingPixelsAmount Entity moving speed.
		 * @param maximumLifePointsAmount Entity maximum life points count.
		 * @param timeBetweenShots How many milliseconds to wait between two shots.
		 * @param firingSoundId The sound to play when the entity shoots.
		 */
		FightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, AudioManager::SoundId firingSoundId): MovingEntity(x, y, textureId, movingPixelsAmount)
		{
			_lifePointsAmount = maximumLifePointsAmount;
			_maximumLifePointsAmount = maximumLifePointsAmount;
			
			// Player and enemies collide with walls and enemy spawners
			_collisionBlockContent = LevelManager::BLOCK_CONTENT_WALL | LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
			
			// Cache the offset to add to entity coordinates to make fired bullets start from where the cannon is
			Texture *pointerBulletTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLET); // Use textures to avoid instantiate a bullet to get its dimensions (all bullets have same dimensions)
			// Assume that entity is faced to the direction the bullet is fired
			_bulletStartingPositionOffsetUpX = (_pointerTexture->getWidth() - pointerBulletTexture->getWidth()) / 2;
			_bulletStartingPositionOffsetUpY = -pointerBulletTexture->getHeight();
			_bulletStartingPositionOffsetDownX = _bulletStartingPositionOffsetUpX;
			_bulletStartingPositionOffsetDownY = _pointerTexture->getHeight(); // Entity is facing left, so its horizontal width is its height
			_bulletStartingPositionOffsetLeftX = _bulletStartingPositionOffsetUpY;
			_bulletStartingPositionOffsetLeftY = _bulletStartingPositionOffsetUpX - 1; // -1 to adjust the rotated bullet texture (TODO how to determine this ?)
			_bulletStartingPositionOffsetRightX = _bulletStartingPositionOffsetDownY; // Entity is facing right, so its horizontal width is its height
			_bulletStartingPositionOffsetRightY = _bulletStartingPositionOffsetLeftY;
			
			_timeBetweenShots = timeBetweenShots;
			_lastShotTime = 0; // Allow to shoot immediately
			_firingSoundId = firingSoundId;
		}
		
		/** Free allocated resources. */
		virtual ~FightingEntity() {};
		
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
		
		/** Tell if the entity is dead or alive.
		 * @return 1 if the entity has no more life point,
		 * @return 0 if the entity is still alive.
		 */
		inline int isDead()
		{
			if (_lifePointsAmount == 0) return 1;
			return 0;
		}
		
		/** Generate a bullet facing the entity direction.
		 * @return A valid pointer if the entity was allowed to shot,
		 * @return NULL if the entity could not shoot (no more ammunition, slower fire rate...).
		 */
		virtual MovingEntityBullet *shoot()
		{
			int bulletStartingPositionOffsetX, bulletStartingPositionOffsetY;
			
			// Allow to shoot only if enough time elapsed since last shot
			if (SDL_GetTicks() - _lastShotTime >= _timeBetweenShots)
			{
				// Select the right offset according to entity direction
				switch (_facingDirection)
				{
					case DIRECTION_UP:
						bulletStartingPositionOffsetX = _bulletStartingPositionOffsetUpX;
						bulletStartingPositionOffsetY = _bulletStartingPositionOffsetUpY;
						break;
						
					case DIRECTION_DOWN:
						bulletStartingPositionOffsetX = _bulletStartingPositionOffsetDownX;
						bulletStartingPositionOffsetY = _bulletStartingPositionOffsetDownY;
						break;
						
					case DIRECTION_LEFT:
						bulletStartingPositionOffsetX = _bulletStartingPositionOffsetLeftX;
						bulletStartingPositionOffsetY = _bulletStartingPositionOffsetLeftY;
						break;
						
					case DIRECTION_RIGHT:
						bulletStartingPositionOffsetX = _bulletStartingPositionOffsetRightX;
						bulletStartingPositionOffsetY = _bulletStartingPositionOffsetRightY;
						break;
						
					default:
						LOG_ERROR("Entity is facing a non-existing direction (direction ID : %d).", _facingDirection);
						return NULL;
				}
				
				MovingEntityBullet *pointerBullet = _fireBullet(_positionRectangle.x + bulletStartingPositionOffsetX, _positionRectangle.y + bulletStartingPositionOffsetY);
				
				// Get time after having generated the bullet, in case this takes more than 1 millisecond
				_lastShotTime = SDL_GetTicks();
				
				// Play firing sound
				AudioManager::playSound(_firingSoundId);
				
				return pointerBullet;
			}
			else return NULL; // No shot allowed
		}
};

#endif