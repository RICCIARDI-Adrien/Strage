/** @file AnimatedTexture.cpp
 * See AnimatedTexture.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AnimatedTexture.hpp>
#include <Log.hpp>
#include <Renderer.hpp>

AnimatedTexture::AnimatedTexture(SDL_Texture *pointerSDLTexture, int imagesCount, int framesPerImageCount): Texture(pointerSDLTexture)
{
	_currentImageIndex = 0;
	_framesCounter = 0;
	_imagesCount = imagesCount;
	_framesPerImageCount = framesPerImageCount;
	
	// Determine a single image width
	_width /= imagesCount; // Texture() constructor computed total texture width yet, so use this value then adjust it
	LOG_DEBUG("Created animated texture. Single image width : %d, single image height : %d.", _width, _height);
}

int AnimatedTexture::render(int x, int y)
{
	SDL_Rect positionRectangle, displayingRectangle;
	
	// Should the next image be displayed ?
	_framesCounter++;
	if (_framesCounter >= _framesPerImageCount)
	{
		// Stop displaying if the animation end has been reached (TODO handle looping animations if needed)
		if (_currentImageIndex >= _imagesCount) return 1;
		else _currentImageIndex++;
		
		_framesCounter = 0;
	}
	
	// Set texture position on display
	positionRectangle.x = x;
	positionRectangle.y = y;
	positionRectangle.w = _width;
	positionRectangle.h = _height;
	
	// Determine the part of the sprite to display
	displayingRectangle.x = _currentImageIndex * _width;
	displayingRectangle.y = 0;
	displayingRectangle.w = _width;
	displayingRectangle.h = _height;
	
	SDL_RenderCopy(Renderer::pointerRenderer, _pointerSDLTexture, &displayingRectangle, &positionRectangle);
	
	return 0;
}
