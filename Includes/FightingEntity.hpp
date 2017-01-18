#ifndef HPP_FIGHTING_ENTITY_HPP
#define HPP_FIGHTING_ENTITY_HPP

#include <AudioManager.hpp>
#include <MovableEntity.hpp>
#include <MovableEntityBullet.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntity
 * A movable entity that has life points and can shoot bullets.
 * @author Adrien RICCIARDI
 */
class FightingEntity: public MovableEntity
{
	protected:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** Add this offset to the entity coordinates to make the fired bullet start from the entity center. */
		int _bulletStartingPositionOffset;
		
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
		virtual MovableEntityBullet *_fireBullet(int x, int y) = 0;
		
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
		FightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, AudioManager::SoundId firingSoundId): MovableEntity(x, y, textureId, movingPixelsAmount)
		{
			_lifePointsAmount = maximumLifePointsAmount;
			_maximumLifePointsAmount = maximumLifePointsAmount;
			
			// Player and enemies collide with walls and enemy spawners
			_collisionBlockContent = LevelManager::BLOCK_CONTENT_WALL | LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
			
			// Cache the offset to add to entity coordinates to make fired bullets start from entity center
			Texture *pointerBulletTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLET); // Use textures to avoid instantiate a bullet to get its dimensions (all bullets have same dimensions)
			_bulletStartingPositionOffset = (_pointerTexture->getWidth() - pointerBulletTexture->getWidth()) / 2; // This works in all fighting entity facing directions because fighting entity textures are appositely circular
			_timeBetweenShots = timeBetweenShots;
			_lastShotTime = 0; // Allow to shoot immediately
			_firingSoundId = firingSoundId;
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
		virtual MovableEntityBullet *shoot()
		{
			// Allow to shoot only if enough time elapsed since last shot
			if (SDL_GetTicks() - _lastShotTime >= _timeBetweenShots)
			{
				MovableEntityBullet *pointerBullet = _fireBullet(_positionRectangle.x + _bulletStartingPositionOffset, _positionRectangle.y + _bulletStartingPositionOffset);
				
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