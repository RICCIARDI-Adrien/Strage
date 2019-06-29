/** @file PlayerFightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_PLAYER_FIGHTING_ENTITY_HPP
#define HPP_PLAYER_FIGHTING_ENTITY_HPP

#include <FightingEntity.hpp>

/** @class PlayerFightingEntity
 * The unique player, controlled by the human player.
 */
class PlayerFightingEntity: public FightingEntity
{
	public:
		/** Create the player.
		 * @param x X coordinate where to spawn the player on the map.
		 * @param y Y coordinate where to spawn the player on the map.
		 */
		PlayerFightingEntity(int x, int y);
		
		/** Free allocated resources. */
		virtual ~PlayerFightingEntity();
		
		// No need for documentation because it is the same as parent function
		virtual bool modifyLife(int lifePointsAmount);
		
		// No need for documentation because it is the same as parent function
		virtual BulletMovingEntity *shoot();
		
		/** Generate a mortar shell facing the entity direction and play the associated effect.
		 * @return A valid pointer if the entity was allowed to shot,
		 * @return NULL if the entity could not shoot (no more ammunition, slower fire rate...).
		 */
		virtual BulletMovingEntity *shootSecondaryFire();
		
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
		virtual void render();
		
		/** Check for a pickable item on the underlaying block and get it if possible.
		 * @return 0 if the game should continue,
		 * @return 2 if the next level must be loaded.
		 */
		virtual int update();
		
	protected:
		// No need for documentation because it is the same as parent function
		virtual BulletMovingEntity *_fireBullet(int x, int y);
		
	private:
		/** All available bonuses. */
		typedef enum
		{
			BONUS_NONE, //!< No bonus is currently enabled.
			BONUS_MACHINE_GUN, //!< Shoot faster without consuming ammunition.
			BONUS_BULLETPROOF_VEST //!< Enemy can't damage the player.
		} Bonus;
		
		/** Display the player at this horizontal display coordinate. */
		int _renderingX;
		/** Display the player at this vertical display coordinate. */
		int _renderingY;
		
		/** How many ammunition the player owns. */
		int _ammunitionAmount; // Only player has limited ammunition, because what could do a munitions-less enemy ?
		
		/** When was the last shot fired (in milliseconds). This is part of the fire rate mechanism. */
		unsigned int _secondaryFireLastShotTime;
		/** How many milliseconds to wait between two shots. */
		unsigned int _secondaryFireTimeBetweenShots;
		/** Tell whether the time between two shoots has elapsed. */
		bool _isSecondaryShootReloadingTimeElapsed = true; // Allow the player to immediately shoot on spawn
		
		/** How many time remaining before the bonus becomes inactive (in milliseconds). */
		int _bonusRemainingTime = 0;
		/** The currently active bonus. */
		Bonus _currentActiveBonus = BONUS_NONE;
		
		/** Offset to add to entity coordinates to fire the bullet in the entity facing direction. */
		SDL_Point _secondaryFireStartingPositionOffsets[DIRECTIONS_COUNT]; // Offsets are in the same order than Direction enum
		
		/** Offset to add to entity coordinates to put the shoot firing effect in front of the entity cannon. */
		SDL_Point _secondaryFireFiringEffectStartingPositionOffsets[DIRECTIONS_COUNT]; // Offsets are in the same order than Direction enum
};

/** The unique player, which must be accessible to any other module. */
extern PlayerFightingEntity *pointerPlayer;

#endif
