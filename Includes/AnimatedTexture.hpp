/** @file AnimatedTexture.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_ANIMATED_TEXTURE_HPP
#define HPP_ANIMATED_TEXTURE_HPP

#include <Texture.hpp>

/** @class AnimatedTexture
 * Display a sequence of animated images.
 */
class AnimatedTexture: public Texture
{
	protected:
		/** How many static images are contained in the provided image file. */
		int _imagesCount;
		/** The currently displayed image. */
		int _currentImageIndex;
		
		/** How much time each static image must be displayed on the screen. Time is computed taking into account engine frames per second. */
		int _framesPerImageCount;
		/** Incremented by one on each engine frame, tell when to display the next texture. */
		int _framesCounter;
	
	public:
		/** Create an animated texture.
		 * @param pointerSDLTexture The SDL texture to use.
		 * @param imagesCount How many single images are present in the provided texture.
		 * @param framesPerImageCount How many game frames to wait between each single image displaying.
		 * @note Texture starts playing from the first image.
		 */
		AnimatedTexture(SDL_Texture *pointerSDLTexture, int imagesCount, int framesPerImageCount): Texture(pointerSDLTexture)
		{
			_currentImageIndex = 0;
			_framesCounter = 0;
			_imagesCount = imagesCount;
			_framesPerImageCount = framesPerImageCount;
			
			// Determine a single image width
			_width /= imagesCount; // Texture() constructor computed total texture width yet, so use this value then adjust it
			LOG_DEBUG("Created animated texture. Single image width : %d, single image height : %d.", _positionRectangle.w, _positionRectangle.h);
		}
		
		/** Free allocated resources. */
		virtual ~AnimatedTexture() {}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 * @return 0 if the animation is not finished,
		 * @return 1 if the animation has finished playing.
		 */
		virtual int render(int x, int y)
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
};

#endif
