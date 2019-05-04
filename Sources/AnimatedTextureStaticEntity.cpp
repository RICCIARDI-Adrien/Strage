/** @file AnimatedTextureStaticEntity.cpp
 * See AnimatedTextureStaticEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTextureStaticEntity.hpp>
#include <Renderer.hpp>

AnimatedTextureStaticEntity::AnimatedTextureStaticEntity(int x, int y, AnimatedTexture *pointerTexture): StaticEntity(x, y, pointerTexture)
{
	_isAnimationTerminated = 0;
}

AnimatedTextureStaticEntity::~AnimatedTextureStaticEntity()
{
	delete _pointerTexture;
}

void AnimatedTextureStaticEntity::render()
{
	// Display the animation current image
	if (_pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY) != 0) _isAnimationTerminated = 1;
}

int AnimatedTextureStaticEntity::update()
{
	return _isAnimationTerminated;
}
