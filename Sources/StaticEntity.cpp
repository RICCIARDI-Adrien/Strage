/** @file StaticEntity.cpp
 * See StaticEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <StaticEntity.hpp>

StaticEntity::StaticEntity(int x, int y, Texture *pointerTexture)
{
	assert(pointerTexture != NULL);
	
	_positionRectangle.x = x;
	_positionRectangle.y = y;
	
	// Cache some values to speed up processing
	_pointerTexture = pointerTexture;
	_positionRectangle.w = pointerTexture->getWidth();
	_positionRectangle.h = pointerTexture->getHeight();
}

void StaticEntity::render()
{
	_pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY);
}

int StaticEntity::getX()
{
	return _positionRectangle.x;
}

void StaticEntity::setX(int x)
{
	_positionRectangle.x = x;
}

int StaticEntity::getY()
{
	return _positionRectangle.y;
}

void StaticEntity::setY(int y)
{
	_positionRectangle.y = y;
}

SDL_Rect *StaticEntity::getPositionRectangle()
{
	return &_positionRectangle;
}
