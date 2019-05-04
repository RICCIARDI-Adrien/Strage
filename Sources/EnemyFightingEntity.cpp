/** @file EnemyFightingEntity.cpp
 * See EnemyFightingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTextureStaticEntity.hpp>
#include <AudioManager.hpp>
#include <BulletMovingEntity.hpp>
#include <Configuration.hpp>
#include <cstdlib>
#include <EnemyFightingEntity.hpp>
#include <LevelManager.hpp>
#include <Log.hpp>
#include <PlayerFightingEntity.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

EnemyFightingEntity::EnemyFightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, TextureManager::TextureId bulletTextureId, EffectManager::EffectId firingEffectId, EffectManager::EffectId explosionEffectId): FightingEntity(x, y, textureId, movingPixelsAmount, maximumLifePointsAmount, timeBetweenShots, bulletTextureId, firingEffectId)
{
	// Enemies collide between them too
	_collisionBlockContent |= LevelManager::BLOCK_CONTENT_ENEMY;
	
	// Initialize spotting rectangle to around the entity
	_spottingRectangle.w = Renderer::displayWidth * 2; // Thus, the player staying at on side of the display will be spotted by an enemy located at the display other side
	_spottingRectangle.h = Renderer::displayHeight * 2;
	_spottingRectangle.x = _positionRectangles[DIRECTION_UP].x - ((_spottingRectangle.w - _positionRectangles[DIRECTION_UP].w) / 2);
	_spottingRectangle.y = _positionRectangles[DIRECTION_UP].y - ((_spottingRectangle.h - _positionRectangles[DIRECTION_UP].h) / 2);
	
	// Cache effects
	_explosionEffectId = explosionEffectId;
	
	// Get a bullet width
	Texture *pointerBulletTexture = TextureManager::getTextureFromId(bulletTextureId); // Use textures to avoid instantiate a bullet to get its dimensions
	int bulletWidth = pointerBulletTexture->getWidth();
	
	// Cache all shooting rectangles
	// Up direction
	_shootingRectangles[DIRECTION_UP].x = _positionRectangles[DIRECTION_UP].x + ((_positionRectangles[DIRECTION_UP].w - bulletWidth) / 2);
	_shootingRectangles[DIRECTION_UP].y = (_positionRectangles[DIRECTION_UP].y + (_positionRectangles[DIRECTION_UP].h / 2)) - (Renderer::displayHeight / 2);
	_shootingRectangles[DIRECTION_UP].w = bulletWidth;
	_shootingRectangles[DIRECTION_UP].h = (Renderer::displayHeight / 2) - (_positionRectangles[DIRECTION_UP].h / 2); // Make the enemy always visible when it starts shooting the player, make the rectangle height end at the beginning of the enemy texture
	// Down direction
	_shootingRectangles[DIRECTION_DOWN].x = _positionRectangles[DIRECTION_UP].x + ((_positionRectangles[DIRECTION_UP].w - bulletWidth) / 2);
	_shootingRectangles[DIRECTION_DOWN].y = _positionRectangles[DIRECTION_UP].y + _positionRectangles[DIRECTION_UP].h;
	_shootingRectangles[DIRECTION_DOWN].w = bulletWidth;
	_shootingRectangles[DIRECTION_DOWN].h = (Renderer::displayHeight / 2) - (_positionRectangles[DIRECTION_UP].h / 2);
	// Left direction
	_shootingRectangles[DIRECTION_LEFT].x = _positionRectangles[DIRECTION_UP].x + (_positionRectangles[DIRECTION_UP].w / 2) - (Renderer::displayWidth / 2);
	_shootingRectangles[DIRECTION_LEFT].y = _positionRectangles[DIRECTION_UP].y + ((_positionRectangles[DIRECTION_UP].h - bulletWidth) / 2);
	_shootingRectangles[DIRECTION_LEFT].w = (Renderer::displayWidth / 2) - (_positionRectangles[DIRECTION_UP].h / 2);
	_shootingRectangles[DIRECTION_LEFT].h = bulletWidth;
	// Right direction
	_shootingRectangles[DIRECTION_RIGHT].x = _positionRectangles[DIRECTION_UP].x + _positionRectangles[DIRECTION_UP].w;
	_shootingRectangles[DIRECTION_RIGHT].y = _positionRectangles[DIRECTION_UP].y + ((_positionRectangles[DIRECTION_UP].h - bulletWidth) / 2);
	_shootingRectangles[DIRECTION_RIGHT].w = (Renderer::displayWidth / 2) - (_positionRectangles[DIRECTION_UP].h / 2);
	_shootingRectangles[DIRECTION_RIGHT].h = bulletWidth;
	
	// Set block under enemy center as containing an enemy
	_setBlockEnemyContent(1);
}

EnemyFightingEntity::~EnemyFightingEntity() {}

void EnemyFightingEntity::setX(int x)
{
	// Adjust shooting rectangles
	int movedPixelsAmount = x - _positionRectangles[DIRECTION_UP].x; // Compute the amount of pixels that have been moved (i.e. an offset) to avoid to recompute all collision rectangles
	for (int i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].x += movedPixelsAmount;
	
	// Adjust position rectangles
	MovingEntity::setX(x);
}

void EnemyFightingEntity::setY(int y)
{
	// Adjust shooting rectangles
	int movedPixelsAmount = y - _positionRectangles[DIRECTION_UP].y; // Compute the amount of pixels that have been moved (i.e. an offset) to avoid to recompute all collision rectangles
	for (int i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].y += movedPixelsAmount;
	
	// Adjust position rectangles
	MovingEntity::setY(y);
}

int EnemyFightingEntity::moveToUp()
{
	// Remove enemy presence from current block
	_setBlockEnemyContent(0);
	int movedPixelsAmount = MovingEntity::moveToUp();
	// Set enemy presence in new block
	_setBlockEnemyContent(1);
	
	// Adjust rectangles coordinates to take this move into account
	_spottingRectangle.y -= movedPixelsAmount;
	for (int i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].y -= movedPixelsAmount;
	
	return movedPixelsAmount;
}

int EnemyFightingEntity::moveToDown()
{
	// Remove enemy presence from current block
	_setBlockEnemyContent(0);
	int movedPixelsAmount = MovingEntity::moveToDown();
	// Set enemy presence in new block
	_setBlockEnemyContent(1);
	
	// Adjust rectangles coordinates to take this move into account
	_spottingRectangle.y += movedPixelsAmount;
	for (int i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].y += movedPixelsAmount;
	
	return movedPixelsAmount;
}

int EnemyFightingEntity::moveToLeft()
{
	// Remove enemy presence from current block
	_setBlockEnemyContent(0);
	int movedPixelsAmount = MovingEntity::moveToLeft();
	// Set enemy presence in new block
	_setBlockEnemyContent(1);
	
	// Adjust rectangles coordinates to take this move into account
	_spottingRectangle.x -= movedPixelsAmount;
	for (int i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].x -= movedPixelsAmount;
	
	return movedPixelsAmount;
}

int EnemyFightingEntity::moveToRight()
{
	// Remove enemy presence from current block
	_setBlockEnemyContent(0);
	int movedPixelsAmount = MovingEntity::moveToRight();
	// Set enemy presence in new block
	_setBlockEnemyContent(1);
	
	// Adjust rectangles coordinates to take this move into account
	_spottingRectangle.x += movedPixelsAmount;
	for (int i = 0; i < DIRECTIONS_COUNT; i++) _shootingRectangles[i].x += movedPixelsAmount;
	
	return movedPixelsAmount;
}

int EnemyFightingEntity::update()
{
	// The entity is dead, remove it
	if (_lifePointsAmount == 0)
	{
		// Remove enemy presence from the block
		_setBlockEnemyContent(0);
		
		// Display explosion
		EffectManager::addEffect(this->getX(), this->getY(), _explosionEffectId);
		
		return 1;
	}
	
	// Nothing to do if the player is not spotted
	if (!SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_spottingRectangle)) return 0;
	
	// Shoot if the player is at sight
	if (_isShootPossible()) return 2;
	
	// If the enemy can't shoot, it must move to come close enough to the player
	Direction playerDirection;
	if (!_getPlayerDirection(&playerDirection)) // The best direction is the one to reach the player
	{
		LOG_DEBUG("Enemy can't shoot but can't move either.");
		return 0;
	}
	
	// Try to move in the best direction
	int movedPixelsAmount = move(playerDirection);
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

void EnemyFightingEntity::render()
{
	MovingEntity::render();
	
	// Display the various rectangles in debug mode
	#if CONFIGURATION_LOG_LEVEL == 3
	{
		// Spotting rectangle
		SDL_SetRenderDrawColor(Renderer::pointerRenderer, 0, 255, 0, 255);
		
		SDL_Rect renderingRectangle;
		renderingRectangle.x = _spottingRectangle.x - Renderer::displayX;
		renderingRectangle.y = _spottingRectangle.y - Renderer::displayY;
		renderingRectangle.w = _spottingRectangle.w;
		renderingRectangle.h = _spottingRectangle.h;
		SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
		
		// Shooting rectangles
		SDL_SetRenderDrawColor(Renderer::pointerRenderer, 255, 0, 0, 255);
		
		renderingRectangle.x = _shootingRectangles[DIRECTION_UP].x - Renderer::displayX;
		renderingRectangle.y = _shootingRectangles[DIRECTION_UP].y - Renderer::displayY;
		renderingRectangle.w = _shootingRectangles[DIRECTION_UP].w;
		renderingRectangle.h = _shootingRectangles[DIRECTION_UP].h;
		SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
		
		renderingRectangle.x = _shootingRectangles[DIRECTION_DOWN].x - Renderer::displayX;
		renderingRectangle.y = _shootingRectangles[DIRECTION_DOWN].y - Renderer::displayY;
		renderingRectangle.w = _shootingRectangles[DIRECTION_DOWN].w;
		renderingRectangle.h = _shootingRectangles[DIRECTION_DOWN].h;
		SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
		
		renderingRectangle.x = _shootingRectangles[DIRECTION_LEFT].x - Renderer::displayX;
		renderingRectangle.y = _shootingRectangles[DIRECTION_LEFT].y - Renderer::displayY;
		renderingRectangle.w = _shootingRectangles[DIRECTION_LEFT].w;
		renderingRectangle.h = _shootingRectangles[DIRECTION_LEFT].h;
		SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
		
		renderingRectangle.x = _shootingRectangles[DIRECTION_RIGHT].x - Renderer::displayX;
		renderingRectangle.y = _shootingRectangles[DIRECTION_RIGHT].y - Renderer::displayY;
		renderingRectangle.w = _shootingRectangles[DIRECTION_RIGHT].w;
		renderingRectangle.h = _shootingRectangles[DIRECTION_RIGHT].h;
		SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
		
		SDL_SetRenderDrawColor(Renderer::pointerRenderer, 0, 0, 0, 255);
	}
	#endif
}

int EnemyFightingEntity::_isShootPossible()
{
	// Is the player in line of sight ?
	if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_UP]))
	{
		// Shoot if the enemy is in the right direction
		if (_facingDirection == DIRECTION_UP)
		{
			if (!LevelManager::isObstaclePresentOnVerticalAxis(pointerPlayer->getY(), getY(), getX())) return 1;
		}
		else turnToUp(); // Face right direction
	}
	else if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_DOWN]))
	{
		if (_facingDirection == DIRECTION_DOWN)
		{
			if (!LevelManager::isObstaclePresentOnVerticalAxis(getY(), pointerPlayer->getY(), getX())) return 1;
		}
		else turnToDown();
	}
	else if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_LEFT]))
	{
		if (_facingDirection == DIRECTION_LEFT)
		{
			if (!LevelManager::isObstaclePresentOnHorizontalAxis(pointerPlayer->getX(), getX(), getY())) return 1;
		}
		else turnToLeft();
	}
	else if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), &_shootingRectangles[DIRECTION_RIGHT]))
	{
		if (_facingDirection == DIRECTION_RIGHT)
		{
			if (!LevelManager::isObstaclePresentOnHorizontalAxis(getX(), pointerPlayer->getX(), getY())) return 1;
		}
		else turnToRight();
	}
	
	return 0;
}

int EnemyFightingEntity::_getPlayerDirection(Direction *pointerDirection)
{
	// Compute both player and enemy centers
	SDL_Rect *pointerPlayerPositionRectangle, *pointerEnemyPositionRectangle;
	pointerPlayerPositionRectangle = pointerPlayer->getPositionRectangle();
	pointerEnemyPositionRectangle = &_positionRectangles[_facingDirection];
	int enemyCenterX = pointerEnemyPositionRectangle->x + (pointerEnemyPositionRectangle->w / 2);
	int enemyCenterY = pointerEnemyPositionRectangle->y + (pointerEnemyPositionRectangle->h / 2);
	
	// Compute a fast distance-like to know if the player is farther horizontally or vertically
	int horizontalDistance = enemyCenterX - (pointerPlayerPositionRectangle->x + (pointerPlayerPositionRectangle->w / 2));
	// Remove sign to allow comparison
	if (horizontalDistance < 0) horizontalDistance = -horizontalDistance;
	int verticalDistance = enemyCenterY - (pointerPlayerPositionRectangle->y + (pointerPlayerPositionRectangle->h / 2));
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

void EnemyFightingEntity::_setBlockEnemyContent(bool isEnemyPresent)
{
	// Cache enemy center coordinates
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	int enemyCenterX = pointerPositionRectangle->x + (pointerPositionRectangle->w / 2);
	int enemyCenterY = pointerPositionRectangle->y + (pointerPositionRectangle->h / 2);
	
	// Get current block content
	int blockContent = LevelManager::getBlockContent(enemyCenterX, enemyCenterY);
	
	// Set or reset enemy flag
	if (isEnemyPresent) blockContent |= LevelManager::BLOCK_CONTENT_ENEMY;
	else blockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY;
	
	// Set new block content
	LevelManager::setBlockContent(enemyCenterX, enemyCenterY, blockContent);
}
