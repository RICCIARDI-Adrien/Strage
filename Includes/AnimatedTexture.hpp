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
		/** TODO */
		int _currentImageIndex;
		
		/** How much time each static image must be displayed on the screen. Time is computed taking into account engine frames per second. */
		int _framesPerImageCount;
		/** Incremented by one on each engine frame, tell when to display the next texture. */
		int _framesCounter;
	
	public:
		/** TODO */
		AnimatedTexture(const char *pointerStringFileName, int isRleCompressionEnabled, int imagesCount, int framesPerImageCount)
		{
			unsigned int pixelFormat;
			int access, spriteWidth;
			
			// Try to load the texture
			_pointerTexture = _loadFromBitmap(pointerStringFileName, isRleCompressionEnabled);
			if (_pointerTexture == NULL) exit(-1);
			
			// Cache width and height parameters
			if (SDL_QueryTexture(_pointerTexture, &pixelFormat, &access, &spriteWidth, &_positionRectangle.h) != 0)
			{
				LOG_ERROR("Failed to query texture information (%s).", SDL_GetError());
				exit(-1);
			}
			
			_framesPerImageCount = framesPerImageCount;
			
			// Determine a single image width
			_imagesCount = imagesCount;
			_positionRectangle.w = spriteWidth / imagesCount;
			LOG_DEBUG("Loaded animated texture '%s'. Image width : %d.", pointerStringFileName, _positionRectangle.w);
		}
		
		/** Free allocated resources. */
		virtual ~AnimatedTexture() {}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 */
		virtual void render(int x, int y)
		{
			SDL_Rect displayingRectangle;
			
			// Should the next image be displayed ?
			_framesCounter++;
			if (_framesCounter >= _framesPerImageCount)
			{
				// Restart animation from the beginning if end has been reached
				if (_currentImageIndex >= _imagesCount) _currentImageIndex = 0;
				else _currentImageIndex++;
				
				_framesCounter = 0;
			}
			
			// Determine the part of the sprite to display
			displayingRectangle.x = _currentImageIndex * _positionRectangle.w;
			displayingRectangle.y = 0;
			displayingRectangle.w = _positionRectangle.w;
			displayingRectangle.h = _positionRectangle.h;
			
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			SDL_RenderCopy(Renderer::pointerRenderer, _pointerTexture, &displayingRectangle, &_positionRectangle);
		}
		
		/** Get the SDL texture which can be rendered using SDL API.
		 * @return The SDL texture.
		 */
		inline SDL_Texture *getTexture() // TODO ?
		{
			return _pointerTexture;
		}
};

#endif
