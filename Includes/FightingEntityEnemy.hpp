#ifndef HPP_FIGHTING_ENTITY_ENEMY_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_HPP

#include <cstdlib>
#include <FightingEntity.hpp>
#include <FightingEntityPlayer.hpp>
#include <MovableEntityBullet.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

/** @class FightingEntityEnemy
 * An enemy attacking the player at sight.
 * @author Adrien RICCIARDI
 */
class FightingEntityEnemy: public FightingEntity
{
	private:
		/** Tell if the enemy can shoot the player or not.
		 * @return 1 if the enemy can shoot,
		 * @return 0 if the player is out of range.
		 */
		int isShootPossible()
		{
			// Is the player in line of sight ? // TODO do not fire if there is a wall between the player and the enemy
			if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_UP]))
			{
				// Shoot if the enemy is in the right direction
				if (_facingDirection == DIRECTION_UP) return 1;
				else turnToUp(); // Face right direction
			}
			else if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_DOWN]))
			{
				if (_facingDirection == DIRECTION_DOWN) return 1;
				else turnToDown();
			}
			else if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_LEFT]))
			{
				if (_facingDirection == DIRECTION_LEFT) return 1;
				else turnToLeft();
			}
			else if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_RIGHT]))
			{
				if (_facingDirection == DIRECTION_RIGHT) return 1;
				else turnToRight();
			}
			
			return 0;
		}
		
		/** Tell in which direction to go to reach the player.
		 * @param pointerDirection On output, contain the player direction.
		 * @return 1 if the enemy should move,
		 * @return 0 if the enemy can remain still.
		 */
		int getPlayerDirection(Direction *pointerDirection)
		{
			int enemyCenterX, enemyCenterY, horizontalDistance, verticalDistance;
			SDL_Rect *pointerPlayerPositionRectangle;
			
			// Compute both player and enemy centers
			pointerPlayerPositionRectangle = pointerPlayer->getPositionRectangle();
			enemyCenterX = _positionRectangle.x + (_positionRectangle.w / 2);
			enemyCenterY = _positionRectangle.y + (_positionRectangle.h / 2);
			
			// Compute a fast distance-like to know if the player is farther horizontally or vertically
			horizontalDistance = enemyCenterX - (pointerPlayerPositionRectangle->x + (pointerPlayerPositionRectangle->w / 2));
			// Remove sign to allow comparison
			if (horizontalDistance < 0) horizontalDistance = -horizontalDistance;
			verticalDistance = enemyCenterY - (pointerPlayerPositionRectangle->y + (pointerPlayerPositionRectangle->h / 2));
			if (verticalDistance < 0) verticalDistance = -verticalDistance;
			
			// Try to come the most closer to the player by moving on the farther direction
			if (horizontalDistance + 50 >= verticalDistance) // Add some distance to the player to avoid enemies collide with player
			{
				// The enemy is too much on the player left to shoot
				if (enemyCenterX < pointerPlayerPositionRectangle->x)
				{
					*pointerDirection = DIRECTION_RIGHT;
					return 1;
				}
				// The enemy if too much on the player right to shoot
				if (enemyCenterX >= pointerPlayerPositionRectangle->x + pointerPlayerPositionRectangle->w)
				{
					*pointerDirection = DIRECTION_LEFT;
					return 1;
				}
			}
			else
			{
				// The enemy if too much on the player top to shoot
				if (enemyCenterY < pointerPlayerPositionRectangle->y)
				{
					*pointerDirection = DIRECTION_DOWN;
					return 1;
				}
				// The enemy if too much on the player bottom to shoot
				if (enemyCenterY >= pointerPlayerPositionRectangle->y + pointerPlayerPositionRectangle->h)
				{
					*pointerDirection = DIRECTION_UP;
					return 1;
				}
			}
			
			return 0;
		}
	
	protected:
		/** The player will be spotted by the enemy if he enters this area. */
		SDL_Rect _spottingRectangle;
		
		/** The enemy will shoot if the player enters this rectangles. */
		SDL_Rect _shootingRectangles[DIRECTIONS_COUNT];
		
		/** Tell if a replacement direction has been chosen or not. */
		int _isReplacementDirectionChosen;
		/** The direction to use when the player direction is blocked by a wall. */
		Direction _replacementDirection;
		
	public:
		/** Spawn a new enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemy(int x, int y): FightingEntity(TextureManager::TEXTURE_ID_ENEMY, x, y, 2, 20, 1000)
		{
			// Initialize spotting rectangle to around the entity
			_spottingRectangle.w = CONFIGURATION_DISPLAY_WIDTH * 2; // Thus, the player staying at on side of the display will be spotted by an enemy located at the display other side
			_spottingRectangle.h = CONFIGURATION_DISPLAY_HEIGHT * 2;
			_spottingRectangle.x = _positionRectangle.x - ((_spottingRectangle.w - _positionRectangle.w) / 2);
			_spottingRectangle.y = _positionRectangle.y - ((_spottingRectangle.h - _positionRectangle.h) / 2);
			
			// Get a bullet width
			Texture *pointerBulletTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_BULLET); // Use textures to avoid instantiate a bullet to get its dimensions
			int bulletWidth = pointerBulletTexture->getWidth();
			
			// Cache all shooting rectangles
			// Up direction
			_shootingRectangles[DIRECTION_UP].x = _positionRectangle.x + ((_positionRectangle.w - bulletWidth) / 2);
			_shootingRectangles[DIRECTION_UP].y = (_positionRectangle.y + (_positionRectangle.h / 2)) - (CONFIGURATION_DISPLAY_HEIGHT / 2);
			_shootingRectangles[DIRECTION_UP].w = bulletWidth;
			_shootingRectangles[DIRECTION_UP].h = CONFIGURATION_DISPLAY_HEIGHT / 2; // Make the enemy always visible when it starts shooting the player
			// Down direction
			_shootingRectangles[DIRECTION_DOWN].x = _positionRectangle.x + ((_positionRectangle.w - bulletWidth) / 2);
			_shootingRectangles[DIRECTION_DOWN].y = _positionRectangle.y + (_positionRectangle.h / 2);
			_shootingRectangles[DIRECTION_DOWN].w = bulletWidth;
			_shootingRectangles[DIRECTION_DOWN].h = CONFIGURATION_DISPLAY_HEIGHT / 2;
			// Left direction
			_shootingRectangles[DIRECTION_LEFT].x = _positionRectangle.x + (_positionRectangle.w / 2) - (CONFIGURATION_DISPLAY_WIDTH / 2);
			_shootingRectangles[DIRECTION_LEFT].y = _positionRectangle.y + ((_positionRectangle.h - bulletWidth) / 2);
			_shootingRectangles[DIRECTION_LEFT].w = CONFIGURATION_DISPLAY_WIDTH / 2;
			_shootingRectangles[DIRECTION_LEFT].h = bulletWidth;
			// Right direction
			_shootingRectangles[DIRECTION_RIGHT].x = _positionRectangle.x + (_positionRectangle.w / 2);
			_shootingRectangles[DIRECTION_RIGHT].y = _positionRectangle.y + ((_positionRectangle.h - bulletWidth) / 2);
			_shootingRectangles[DIRECTION_RIGHT].w = CONFIGURATION_DISPLAY_WIDTH / 2;
			_shootingRectangles[DIRECTION_RIGHT].h = bulletWidth;
		}
		
		/** Free entity allocated resources. */
		virtual ~FightingEntityEnemy() {} // Nothing to free
		
		virtual int moveToUp()
		{
			int movedPixelsAmount, i;
			
			movedPixelsAmount = MovableEntity::moveToUp();
			
			// Adjust rectangles coordinates to take this move into account
			_spottingRectangle.y -= movedPixelsAmount;
			for (i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].y -= movedPixelsAmount;
			
			return movedPixelsAmount;
		}
		
		virtual int moveToDown()
		{
			int movedPixelsAmount, i;
			
			movedPixelsAmount = MovableEntity::moveToDown();
			
			// Adjust rectangles coordinates to take this move into account
			_spottingRectangle.y += movedPixelsAmount;
			for (i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].y += movedPixelsAmount;
			
			return movedPixelsAmount;
		}
		
		virtual int moveToLeft()
		{
			int movedPixelsAmount, i;
			
			movedPixelsAmount = MovableEntity::moveToLeft();
			
			// Adjust rectangles coordinates to take this move into account
			_spottingRectangle.x -= movedPixelsAmount;
			for (i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].x -= movedPixelsAmount;
			
			return movedPixelsAmount;
		}
		
		virtual int moveToRight()
		{
			int movedPixelsAmount, i;
			
			movedPixelsAmount = MovableEntity::moveToRight();
			
			// Adjust rectangles coordinates to take this move into account
			_spottingRectangle.x += movedPixelsAmount;
			for (i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].x += movedPixelsAmount;
			
			return movedPixelsAmount;
		}
		
		/** Update enemy artificial intelligence.
		 * @return 0 if the enemy must be kept alive,
		 * @return 1 if the enemy is dead and must be removed,
		 * @return 2 if the enemy is alive and wants to shoot (call the shoot() method).
		 */
		virtual int update()
		{
			Direction playerDirection;
			int movedPixelsAmount;
			
			// The entity is dead, remove it
			if (_lifePointsAmount == 0) return 1;
			
			// Nothing to do if the player is not spotted
			if (!SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_spottingRectangle)) return 0;
			
			// Shoot if the player is at sight
			if (isShootPossible()) return 2;
			
			// If the enemy can't shoot, it must move to come close enough to the player
			if (!getPlayerDirection(&playerDirection)) // The best direction is the one to reach the player
			{
				LOG_INFORMATION("Enemy can't shoot but can't move either.\n");
				return 0;
			}
			
			// Try to move in the best direction
			movedPixelsAmount = move(playerDirection);
			if (movedPixelsAmount > 0) _isReplacementDirectionChosen = 0; // Enemy can move in its preferred direction, no need to use a replacement one
			else
			{
				// Choose a replacement direction different from the best one (which is obstructed by a wall)
				if (!_isReplacementDirectionChosen)
				{
					do
					{
						_replacementDirection = (Direction) (rand() % DIRECTIONS_COUNT);
					} while (_replacementDirection == playerDirection);
				}
				
				// Try to move in the chosen direction
				movedPixelsAmount = move(_replacementDirection);
				if (movedPixelsAmount == 0) _isReplacementDirectionChosen = 0; // There is a wall here too... More luck next tick ! TODO : for loop to make sure all enemies move at each tick ?
				else _isReplacementDirectionChosen = 1;
			}
			
			return 0;
		}
		
		virtual void render()
		{
			MovableEntity::render();
			
			// Display the various rectangles in debug mode
			#if CONFIGURATION_LOG_LEVEL == 3
			{
				SDL_Rect renderingRectangle;
				
				// Spotting rectangle
				SDL_SetRenderDrawColor(Renderer::pointerMainRenderer, 0, 255, 0, 255);
				
				renderingRectangle.x = _spottingRectangle.x - Renderer::displayX;
				renderingRectangle.y = _spottingRectangle.y - Renderer::displayY;
				renderingRectangle.w = _spottingRectangle.w;
				renderingRectangle.h = _spottingRectangle.h;
				SDL_RenderDrawRect(Renderer::pointerMainRenderer, &renderingRectangle);
				
				// Shooting rectangles
				SDL_SetRenderDrawColor(Renderer::pointerMainRenderer, 255, 0, 0, 255);
				
				renderingRectangle.x = _shootingRectangles[DIRECTION_UP].x - Renderer::displayX;
				renderingRectangle.y = _shootingRectangles[DIRECTION_UP].y - Renderer::displayY;
				renderingRectangle.w = _shootingRectangles[DIRECTION_UP].w;
				renderingRectangle.h = _shootingRectangles[DIRECTION_UP].h;
				SDL_RenderDrawRect(Renderer::pointerMainRenderer, &renderingRectangle);
				
				renderingRectangle.x = _shootingRectangles[DIRECTION_DOWN].x - Renderer::displayX;
				renderingRectangle.y = _shootingRectangles[DIRECTION_DOWN].y - Renderer::displayY;
				renderingRectangle.w = _shootingRectangles[DIRECTION_DOWN].w;
				renderingRectangle.h = _shootingRectangles[DIRECTION_DOWN].h;
				SDL_RenderDrawRect(Renderer::pointerMainRenderer, &renderingRectangle);
				
				renderingRectangle.x = _shootingRectangles[DIRECTION_LEFT].x - Renderer::displayX;
				renderingRectangle.y = _shootingRectangles[DIRECTION_LEFT].y - Renderer::displayY;
				renderingRectangle.w = _shootingRectangles[DIRECTION_LEFT].w;
				renderingRectangle.h = _shootingRectangles[DIRECTION_LEFT].h;
				SDL_RenderDrawRect(Renderer::pointerMainRenderer, &renderingRectangle);
				
				renderingRectangle.x = _shootingRectangles[DIRECTION_RIGHT].x - Renderer::displayX;
				renderingRectangle.y = _shootingRectangles[DIRECTION_RIGHT].y - Renderer::displayY;
				renderingRectangle.w = _shootingRectangles[DIRECTION_RIGHT].w;
				renderingRectangle.h = _shootingRectangles[DIRECTION_RIGHT].h;
				SDL_RenderDrawRect(Renderer::pointerMainRenderer, &renderingRectangle);
				
				SDL_SetRenderDrawColor(Renderer::pointerMainRenderer, 0, 0, 0, 255);
			}
			#endif
		}
};

#endif