#ifndef HPP_PICTURE_HPP
#define HPP_PICTURE_HPP

#include <SDL2/SDL.h>

/** @class Texture
 * Allow to load and display a static picture.
 * @author Adrien RICCIARDI
 */
class Texture
{
	private:
		/** The texture. */
		SDL_Texture *_pointerTexture;
		
		/** Use this rectangle to render the texture (some values are cached this way). */
		SDL_Rect _renderingDestinationRectangle;
	
	protected:
		/** Load a texture from a bitmap file.
		 * @param fileName The bitmap file.
		 * @return a valid pointer if the texture was successfully loaded,
		 * @return NULL if an error occurred.
		 */
		SDL_Texture *loadFromBitmap(const char *fileName);
	
	public:
		/** Create a SDL texture from a picture file.
		 * @param fileName The picture file.
		 */
		Texture(const char *fileName);
		
		/** Free all allocated resources. */
		virtual ~Texture();
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 */
		void render(int x, int y);
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle. Texture is rotated the specified amount of degrees using its center as rotation point.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 * @param rotationAngle Amount of degrees to rotate the texture arount its center.
		 */
		void render(int x, int y, double rotationAngle);
		
		/** Get the texture width in pixels.
		 * @return The texture width.
		 */
		inline int getWidth()
		{
			return _renderingDestinationRectangle.w;
		}
		
		/** Get the texture height in pixels.
		 * @return The texture height.
		 */
		inline int getHeight()
		{
			return _renderingDestinationRectangle.h;
		}
};

#endif