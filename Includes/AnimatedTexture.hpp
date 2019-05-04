/** @file AnimatedTexture.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_ANIMATED_TEXTURE_HPP
#define HPP_ANIMATED_TEXTURE_HPP

#include <SDL2/SDL.h>
#include <Texture.hpp>

/** @class AnimatedTexture
 * Display a sequence of animated images. All images must have the same dimensions.
 */
class AnimatedTexture: public Texture
{
	public:
		/** Create an animated texture.
		 * @param pointerSDLTexture The SDL texture to use.
		 * @param imagesCount How many single images are present in the provided texture.
		 * @param framesPerImageCount How many game frames to wait between each single image displaying.
		 * @param isAnimationLooping Set to true to make the animation loop forever, set to false to display the animation only once.
		 * @note Texture starts playing from the first image.
		 */
		AnimatedTexture(SDL_Texture *pointerSDLTexture, int imagesCount, int framesPerImageCount, bool isAnimationLooping);
		
		/** Free allocated resources. */
		virtual ~AnimatedTexture() {}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 * @return 0 if the animation is not finished,
		 * @return 1 if the animation has finished playing (only if the texture was created with isAnimationLooping set to false).
		 */
		virtual int render(int x, int y);
	
	protected:
		/** How many static images are contained in the provided image file. */
		int _imagesCount;
		/** The currently displayed image. */
		int _currentImageIndex;
		
		/** How much time each static image must be displayed on the screen. Time is computed taking into account engine frames per second. */
		int _framesPerImageCount;
		/** Incremented by one on each engine frame, tell when to display the next texture. */
		int _framesCounter;
		
		/** Tell whether animation should loop forever or stop when the last frame has been displayed. */
		bool _isAnimationLooping;
};

#endif
