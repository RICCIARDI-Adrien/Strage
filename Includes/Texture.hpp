/** @file Texture.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_TEXTURE_HPP
#define HPP_TEXTURE_HPP

#include <SDL2/SDL.h>

/** @class Texture
 * Allow to load and display a static picture.
 */
class Texture
{
	public:
		/** Create a still texture.
		 * @param pointerSDLTexture The SDL texture to display.
		 */
		Texture(SDL_Texture *pointerSDLTexture);
		
		/** Free allocated resources. */
		virtual ~Texture() {}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 * @return Always 0.
		 */
		virtual int render(int x, int y);
		
		/** Get the texture width in pixels.
		 * @return The texture width.
		 */
		inline int getWidth()
		{
			return _width;
		}
		
		/** Get the texture height in pixels.
		 * @return The texture height.
		 */
		inline int getHeight()
		{
			return _height;
		}
		
		/** Get the SDL texture which can be rendered using SDL API.
		 * @return The SDL texture.
		 */
		inline SDL_Texture *getSDLTexture()
		{
			return _pointerSDLTexture;
		}

	protected:
		/** The texture. */
		SDL_Texture *_pointerSDLTexture;
		
		/** Texture width in pixels. */
		int _width;
		/** Texture height in pixels. */
		int _height;
};

#endif
