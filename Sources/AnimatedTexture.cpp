/** @file AnimatedTexture.cpp
 * See AnimatedTexture.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTexture.hpp>
#include <Log.hpp>
#include <Renderer.hpp>

AnimatedTexture::AnimatedTexture(SDL_Texture *pointerSDLTexture, int imagesCount, int framesPerImageCount, bool isAnimationLooping): Texture(pointerSDLTexture)
{
	_currentImageIndex = 0;
	_framesCounter = 0;
	_imagesCount = imagesCount;
	_framesPerImageCount = framesPerImageCount;
	_isAnimationLooping = isAnimationLooping;
	
	// Determine a single image width
	_width /= imagesCount; // Texture() constructor computed total texture width yet, so use this value then adjust it
	LOG_DEBUG("Created animated texture. Single image width : %d, single image height : %d.", _width, _height);
}

int AnimatedTexture::render(int x, int y)
{
	// Should the next image be displayed ?
	_framesCounter++;
	if (_framesCounter >= _framesPerImageCount)
	{
		// Is the animation end reached ?
		_currentImageIndex++;
		if (_currentImageIndex >= _imagesCount)
		{
			if (!_isAnimationLooping) return 1; // Looping mode is disabled, stop displaying if the animation end has been reached
			else _currentImageIndex = 0;
		}
		
		_framesCounter = 0;
	}
	
	// Set texture position on display
	SDL_Rect positionRectangle;
	positionRectangle.x = x;
	positionRectangle.y = y;
	positionRectangle.w = _width;
	positionRectangle.h = _height;
	
	// Render texture only if it is visible on the screen
	if ((x + _width >= 0) && (y + _height >= 0) && (x < Renderer::displayWidth) && (y < Renderer::displayHeight))
	{
		// Determine the part of the sprite to display
		SDL_Rect displayingRectangle;
		displayingRectangle.x = _currentImageIndex * _width;
		displayingRectangle.y = 0;
		displayingRectangle.w = _width;
		displayingRectangle.h = _height;
		
		SDL_RenderCopy(Renderer::pointerRenderer, _pointerSDLTexture, &displayingRectangle, &positionRectangle);
	}
	
	return 0;
}
