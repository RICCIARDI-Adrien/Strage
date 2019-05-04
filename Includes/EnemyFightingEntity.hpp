/** @file EnemyFightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_ENEMY_FIGHTING_ENTITY_HPP
#define HPP_ENEMY_FIGHTING_ENTITY_HPP

#include <EffectManager.hpp>
#include <FightingEntity.hpp>
#include <SDL2/SDL.h>

/** @class EnemyFightingEntity
 * An enemy attacking the player at sight.
 */
class EnemyFightingEntity: public FightingEntity
{
	public:
		/** Spawn a new enemy.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param textureId The texture to use on rendering.
		 * @param movingPixelsAmount Entity moving speed.
		 * @param maximumLifePointsAmount Entity maximum life points count.
		 * @param timeBetweenShots How many milliseconds to wait between two shots.
		 * @param bulletTextureId The "facing up" texture of the bullet fired by the entity.
		 * @param firingEffectId The effect to play when the entity shoots.
		 * @param explosionEffectId The effect to play when the entity explodes.
		 */
		EnemyFightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, TextureManager::TextureId bulletTextureId, EffectManager::EffectId firingEffectId, EffectManager::EffectId explosionEffectId);
		
		/** Free allocated resources. */
		virtual ~EnemyFightingEntity();
		
		// No need for documentation because it is the same as parent function
		virtual void setX(int x);
		
		// No need for documentation because it is the same as parent function
		virtual void setY(int y);
		
		// No need for documentation because it is the same as parent function
		virtual int moveToUp();
		
		// No need for documentation because it is the same as parent function
		virtual int moveToDown();
		
		// No need for documentation because it is the same as parent function
		virtual int moveToLeft();
		
		// No need for documentation because it is the same as parent function
		virtual int moveToRight();
		
		/** Update enemy artificial intelligence.
		 * @return 0 if the enemy must be kept alive,
		 * @return 1 if the enemy is dead and must be removed,
		 * @return 2 if the enemy is alive and wants to shoot (call the shoot() method).
		 */
		virtual int update();
		
		// No need for documentation because it is the same as parent function
		virtual void render();

	protected:
		/** The player will be spotted by the enemy if he enters this area. */
		SDL_Rect _spottingRectangle;
		
		/** The enemy will shoot if the player enters one of these rectangles. */
		SDL_Rect _shootingRectangles[DIRECTIONS_COUNT];
		
		/** Tell if a replacement direction has been chosen or not. */
		int _isReplacementDirectionChosen;
		/** The direction to use when the player direction is blocked by a wall. */
		Direction _replacementDirection;
		
		/** The effect to generate when the enemy explodes. */
		EffectManager::EffectId _explosionEffectId;

	private:
		/** Tell if the enemy can shoot the player or not.
		 * @return 1 if the enemy can shoot,
		 * @return 0 if the player is out of range.
		 */
		int _isShootPossible();
		
		/** Tell in which direction to go to reach the player.
		 * @param pointerDirection On output, contain the player direction.
		 * @return 1 if the enemy should move,
		 * @return 0 if the enemy can remain still.
		 */
		int _getPlayerDirection(Direction *pointerDirection);
		
		/** Set or reset a block flag telling whether an enemy is present on the block.
		 * @param isEnemyPresent Set to true to set the flag, set to false to reset the flag.
		 */
		void _setBlockEnemyContent(bool isEnemyPresent);
};

#endif
