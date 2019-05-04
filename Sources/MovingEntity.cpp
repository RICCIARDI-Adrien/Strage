/** @file MovingEntity.cpp
 * See MovingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.hpp>
#include <LevelManager.hpp>
#include <MovingEntity.hpp>
#include <Renderer.hpp>

MovingEntity::MovingEntity(int x, int y, TextureManager::TextureId facingUpTextureId, int movingPixelsAmount)
{
	// Set all textures and collision rectangles
	int textureId = (int) facingUpTextureId;
	for (int i = 0; i < DIRECTIONS_COUNT; i++)
	{
		// Set the texture facing in the right direction
		_pointerTextures[i] = TextureManager::getTextureFromId((TextureManager::TextureId) textureId);
		textureId++;
		
		// Set collision rectangle
		_positionRectangles[i].x = x;
		_positionRectangles[i].y = y;
		_positionRectangles[i].h = _pointerTextures[i]->getHeight();
		_positionRectangles[i].w = _pointerTextures[i]->getWidth();
	}
	
	_movingPixelsAmount = movingPixelsAmount;
	
	// Collide with walls by default
	_collisionBlockContent = LevelManager::BLOCK_CONTENT_WALL;
	
	// Entity is facing up on spawn
	_facingDirection = DIRECTION_UP;
}

MovingEntity::~MovingEntity() {}

int MovingEntity::getX()
{
	return _positionRectangles[_facingDirection].x;
}

void MovingEntity::setX(int x)
{
	// Set all rectangles X coordinate (do not use a loop to fasten the code)
	_positionRectangles[DIRECTION_UP].x = x;
	_positionRectangles[DIRECTION_DOWN].x = x;
	_positionRectangles[DIRECTION_LEFT].x = x;
	_positionRectangles[DIRECTION_RIGHT].x = x;
}

int MovingEntity::getY()
{
	return _positionRectangles[_facingDirection].y;
}

void MovingEntity::setY(int y)
{
	// Set all rectangles Y coordinate (do not use a loop to fasten the code)
	_positionRectangles[DIRECTION_UP].y = y;
	_positionRectangles[DIRECTION_DOWN].y = y;
	_positionRectangles[DIRECTION_LEFT].y = y;
	_positionRectangles[DIRECTION_RIGHT].y = y;
}

SDL_Rect *MovingEntity::getPositionRectangle()
{
	return &_positionRectangles[_facingDirection];
}

int MovingEntity::moveToUp()
{
	// Check upper border distance against level walls
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	int leftSideDistanceToWall = LevelManager::getDistanceFromUpperBlock(pointerPositionRectangle->x, pointerPositionRectangle->y, _collisionBlockContent);
	int rightSizeDistanceToWall = LevelManager::getDistanceFromUpperBlock(pointerPositionRectangle->x + pointerPositionRectangle->w - 1, pointerPositionRectangle->y, _collisionBlockContent);
	
	// Keep the smaller distance
	int distanceToWall;
	if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
	else distanceToWall = rightSizeDistanceToWall;
	
	// Move if possible
	int movingPixelsAmount;
	if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
	else movingPixelsAmount = distanceToWall;
	
	// Update all rectangles
	_positionRectangles[DIRECTION_UP].y -= movingPixelsAmount;
	_positionRectangles[DIRECTION_DOWN].y -= movingPixelsAmount;
	_positionRectangles[DIRECTION_LEFT].y -= movingPixelsAmount;
	_positionRectangles[DIRECTION_RIGHT].y -= movingPixelsAmount;
	
	// Entity is facing up
	_facingDirection = DIRECTION_UP;
	
	return movingPixelsAmount;
}

int MovingEntity::moveToDown()
{
	// Check downer border distance against level walls
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	int leftSideDistanceToWall = LevelManager::getDistanceFromDownerBlock(pointerPositionRectangle->x, pointerPositionRectangle->y + pointerPositionRectangle->h, _collisionBlockContent);
	int rightSizeDistanceToWall = LevelManager::getDistanceFromDownerBlock(pointerPositionRectangle->x + pointerPositionRectangle->w - 1, pointerPositionRectangle->y + pointerPositionRectangle->h, _collisionBlockContent);
	
	// Keep the smaller distance
	int distanceToWall;
	if (leftSideDistanceToWall < rightSizeDistanceToWall) distanceToWall = leftSideDistanceToWall;
	else distanceToWall = rightSizeDistanceToWall;
	
	// Move if possible
	int movingPixelsAmount;
	if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
	else movingPixelsAmount = distanceToWall;
	
	// Update all rectangles
	_positionRectangles[DIRECTION_UP].y += movingPixelsAmount;
	_positionRectangles[DIRECTION_DOWN].y += movingPixelsAmount;
	_positionRectangles[DIRECTION_LEFT].y += movingPixelsAmount;
	_positionRectangles[DIRECTION_RIGHT].y += movingPixelsAmount;
	
	// Entity is facing down
	_facingDirection = DIRECTION_DOWN;
	
	return movingPixelsAmount;
}

int MovingEntity::moveToLeft()
{
	// Check leftmost border distance against level walls
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	int upperSideDistanceToWall = LevelManager::getDistanceFromLeftmostBlock(pointerPositionRectangle->x, pointerPositionRectangle->y, _collisionBlockContent);
	int downerSideDistanceToWall = LevelManager::getDistanceFromLeftmostBlock(pointerPositionRectangle->x, pointerPositionRectangle->y + pointerPositionRectangle->h - 1, _collisionBlockContent);
	
	// Keep the smaller distance
	int distanceToWall;
	if (upperSideDistanceToWall < downerSideDistanceToWall) distanceToWall = upperSideDistanceToWall;
	else distanceToWall = downerSideDistanceToWall;
	
	// Move if possible
	int movingPixelsAmount;
	if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
	else movingPixelsAmount = distanceToWall;
	
	// Update all rectangles
	_positionRectangles[DIRECTION_UP].x -= movingPixelsAmount;
	_positionRectangles[DIRECTION_DOWN].x -= movingPixelsAmount;
	_positionRectangles[DIRECTION_LEFT].x -= movingPixelsAmount;
	_positionRectangles[DIRECTION_RIGHT].x -= movingPixelsAmount;
	
	// Entity is facing left
	_facingDirection = DIRECTION_LEFT;
	
	return movingPixelsAmount;
}

int MovingEntity::moveToRight()
{
	// Check rightmost border distance against level walls
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	int upperSideDistanceToWall = LevelManager::getDistanceFromRightmostBlock(pointerPositionRectangle->x + pointerPositionRectangle->w, pointerPositionRectangle->y, _collisionBlockContent);
	int downerSideDistanceToWall = LevelManager::getDistanceFromRightmostBlock(pointerPositionRectangle->x + pointerPositionRectangle->w, pointerPositionRectangle->y + pointerPositionRectangle->h - 1, _collisionBlockContent);
	
	// Keep the smaller distance
	int distanceToWall;
	if (upperSideDistanceToWall < downerSideDistanceToWall) distanceToWall = upperSideDistanceToWall;
	else distanceToWall = downerSideDistanceToWall;
	
	// Move if possible
	int movingPixelsAmount;
	if (distanceToWall >= _movingPixelsAmount) movingPixelsAmount = _movingPixelsAmount;
	else movingPixelsAmount = distanceToWall;
	
	// Update all rectangles
	_positionRectangles[DIRECTION_UP].x += movingPixelsAmount;
	_positionRectangles[DIRECTION_DOWN].x += movingPixelsAmount;
	_positionRectangles[DIRECTION_LEFT].x += movingPixelsAmount;
	_positionRectangles[DIRECTION_RIGHT].x += movingPixelsAmount;
	
	// Entity is facing right
	_facingDirection = DIRECTION_RIGHT;
	
	return movingPixelsAmount;
}

int MovingEntity::move(Direction movingDirection)
{
	switch (movingDirection)
	{
		case DIRECTION_UP:
			return moveToUp();
			
		case DIRECTION_DOWN:
			return moveToDown();
			
		case DIRECTION_LEFT:
			return moveToLeft();
			
		case DIRECTION_RIGHT:
			return moveToRight();
			
		default:
			return 0; // Tell that the entity did not move
	}
}

void MovingEntity::turnToUp()
{
	_facingDirection = DIRECTION_UP;
}

void MovingEntity::turnToDown()
{
	_facingDirection = DIRECTION_DOWN;
}

void MovingEntity::turnToLeft()
{
	_facingDirection = DIRECTION_LEFT;
}

void MovingEntity::turnToRight()
{
	_facingDirection = DIRECTION_RIGHT;
}

void MovingEntity::render()
{
	// Display the texture
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	_pointerTextures[_facingDirection]->render(pointerPositionRectangle->x - Renderer::displayX, pointerPositionRectangle->y - Renderer::displayY);
	
	// Display collision rectangle in debug mode
	#if CONFIGURATION_LOG_LEVEL == 3
	{
		SDL_Rect renderingRectangle, *pointerPositionRectangle;
		
		// Position rectangle
		SDL_SetRenderDrawColor(Renderer::pointerRenderer, 0, 255, 255, 255);
		
		pointerPositionRectangle = &_positionRectangles[_facingDirection];
		renderingRectangle.x = pointerPositionRectangle->x - Renderer::displayX;
		renderingRectangle.y = pointerPositionRectangle->y - Renderer::displayY;
		renderingRectangle.w = pointerPositionRectangle->w;
		renderingRectangle.h = pointerPositionRectangle->h;
		SDL_RenderDrawRect(Renderer::pointerRenderer, &renderingRectangle);
		
		// Restore background color
		SDL_SetRenderDrawColor(Renderer::pointerRenderer, 0, 0, 0, 255);
	}
	#endif
}
