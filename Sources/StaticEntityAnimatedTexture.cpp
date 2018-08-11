/** @file StaticEntityAnimatedTexture.cpp
 * See StaticEntityAnimatedTexture.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Renderer.hpp>
#include <StaticEntityAnimatedTexture.hpp>

StaticEntityAnimatedTexture::StaticEntityAnimatedTexture(int x, int y, AnimatedTexture *pointerTexture): StaticEntity(x, y, pointerTexture)
{
	_isAnimationTerminated = 0;
}

StaticEntityAnimatedTexture::~StaticEntityAnimatedTexture()
{
	delete _pointerTexture;
}

void StaticEntityAnimatedTexture::render()
{
	// Display the animation current image
	if (_pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY) != 0) _isAnimationTerminated = 1;
}

int StaticEntityAnimatedTexture::update()
{
	return _isAnimationTerminated;
}
