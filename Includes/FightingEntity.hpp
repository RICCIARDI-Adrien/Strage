/** @file FightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_FIGHTING_ENTITY_HPP
#define HPP_FIGHTING_ENTITY_HPP

#include <BulletMovingEntity.hpp>
#include <EffectManager.hpp>
#include <MovingEntity.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntity
 * A moving entity that has life points and can shoot bullets.
 */
class FightingEntity: public MovingEntity
{
	public:
		/** Initialize life points in addition to parent classes fields.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param textureId The texture to use on rendering.
		 * @param movingPixelsAmount Entity moving speed.
		 * @param maximumLifePointsAmount Entity maximum life points count.
		 * @param timeBetweenShots How many milliseconds to wait between two shots.
		 * @param facingUpBulletTextureId The "facing up" texture of the bullet fired by the entity.
		 * @param firingEffectId The effect to play when the entity shoots.
		 */
		FightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, TextureManager::TextureId facingUpBulletTextureId, EffectManager::EffectId firingEffectId);
		
		/** Free allocated resources. */
		virtual ~FightingEntity();
		
		/** Get the entity life points.
		 * @return Entity life points amount.
		 */
		inline int getLifePointsAmount()
		{
			return _lifePointsAmount;
		}
		
		/** Set the entity life points.
		 * @param lifePointsAmount Entity life points amount.
		 */
		inline void setLifePointsAmount(int lifePointsAmount)
		{
			_lifePointsAmount = lifePointsAmount;
		}
		
		/** Get the entity maximum life points.
		 * @return Entity maximum life points amount.
		 */
		inline int getMaximumLifePointsAmount()
		{
			return _maximumLifePointsAmount;
		}
		
		/** Set the entity maximum life points.
		 * @param lifePointsAmount Entity maximum life points amount.
		 */
		inline void setMaximumLifePointsAmount(int lifePointsAmount)
		{
			_maximumLifePointsAmount = lifePointsAmount;
		}
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 * @return false if the entity life wasn't modified,
		 * @return true if the entity life was modified.
		 */
		virtual bool modifyLife(int lifePointsAmount);
		
		/** Tell if the entity is at maximum life or not.
		 * @return true if the entity can be healed,
		 * @return false if the entity is healthy.
		 */
		inline bool isWounded()
		{
			if (_lifePointsAmount < _maximumLifePointsAmount) return true;
			return false;
		}
		
		/** Tell if the entity is dead or alive.
		 * @return true if the entity has no more life point,
		 * @return false if the entity is still alive.
		 */
		inline bool isDead()
		{
			if (_lifePointsAmount == 0) return true;
			return false;
		}
		
		/** Generate a bullet facing the entity direction and play the associated effect.
		 * @return A valid pointer if the entity was allowed to shot,
		 * @return NULL if the entity could not shoot (no more ammunition, slower fire rate...).
		 */
		virtual BulletMovingEntity *shoot();

	protected:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** Offset to add to entity coordinates to fire the bullet in the entity facing direction. */
		SDL_Point _bulletStartingPositionOffsets[DIRECTIONS_COUNT]; // Offsets are in the same order than Direction enum
		
		/** Offset to add to entity coordinates to put the shoot firing effect in front of the entity cannon. */
		SDL_Point _firingEffectStartingPositionOffsets[DIRECTIONS_COUNT]; // Offsets are in the same order than Direction enum
		
		/** When was the last shot fired (in milliseconds). This is part of the fire rate mechanism. */
		unsigned int _lastShotTime;
		/** How many milliseconds to wait between two shots. */
		unsigned int _timeBetweenShots;
		
		/** The effect to play when the entity shoots. */
		EffectManager::EffectId _firingEffectId;
		
		/** Generate a bullet from the type the enemy fires.
		 * @param x Bullet X coordinate.
		 * @param y Bullet Y coordinate.
		 * @return An enemy-specific bullet.
		 */
		virtual BulletMovingEntity *_fireBullet(int x, int y) = 0;
		
		/** Compute the offset to entity coordinates to spawn a bullet at the cannon location for each entity direction.
		 * @param pointerEntityTexture The entity texture (all textures must have the same dimensions in all entity directions).
		 * @param pointerFacingUpBulletTexture The bullet texture in the "facing up" direction.
		 * @param pointerPositionOffsets On output, contain the computed offsets.
		 */
		void _computeBulletStartingPositionOffsets(Texture *pointerEntityTexture, Texture *pointerFacingUpBulletTexture, SDL_Point *pointerPositionOffsets);
		
		/** Compute the offset to entity coordinates to spawn a muzzle flash effect at the cannon location for each entity direction.
		 * @param pointerEntityTexture The entity texture (all textures must have the same dimensions in all entity directions).
		 * @param firingEffectId The firing effect.
		 * @param pointerPositionOffsets On output, contain the computed offsets.
		 */
		void _computeFiringEffectStartingPositionOffsets(Texture *pointerEntityTexture, EffectManager::EffectId firingEffectId, SDL_Point *pointerPositionOffsets);
};

#endif
