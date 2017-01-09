#ifndef HPP_FIGHTING_ENTITY_ENEMY_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_HPP

#include <FightingEntity.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntityEnemy
 * An enemy attacking the player at sight.
 * @author Adrien RICCIARDI
 */
class FightingEntityEnemy: public FightingEntity
{
	protected:
		/** The player will be spotted by the enemy if he enters this area. */
		SDL_Rect _spottingRectangle;
		
	public:
		/** Spawn a new enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemy(int x, int y): FightingEntity(TextureManager::TEXTURE_ID_ENEMY, x, y, 3, 20, 400)
		{
			// Initialize spotting rectangle to around the entity
			_spottingRectangle.w = CONFIGURATION_DISPLAY_WIDTH * 2; // Thus, the player staying at on side of the display will be spotted by an enemy located at the display other side
			_spottingRectangle.h = CONFIGURATION_DISPLAY_HEIGHT * 2;
			_spottingRectangle.x = (_spottingRectangle.w - _positionRectangle.w) / 2;
			_spottingRectangle.y = (_spottingRectangle.h - _positionRectangle.h) / 2;
		}
		
		// TODO
		virtual int update() { return 0; }
};

#endif