#ifndef HPP_PICTURE_HPP
#define HPP_PICTURE_HPP

#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>

/** @class Texture
 * Allow to load and display a static picture.
 * @author Adrien RICCIARDI
 */
class Texture
{
	protected:
		/** The texture. */
		SDL_Texture *_pointerTexture;
		
		/** Use this rectangle to render the texture (some values are cached this way). */
		SDL_Rect _positionRectangle;
	
		/** Load a texture from a bitmap file.
		 * @param fileName The bitmap file.
		 * @param isRleCompressionEnabled Set to 1 to enable the RLE compression, resulting in faster rendering on highly transparent pictures.
		 * @return a valid pointer if the texture was successfully loaded,
		 * @return NULL if an error occurred.
		 */
		SDL_Texture *_loadFromBitmap(const char *fileName, int isRleCompressionEnabled)
		{
			// Try to load bitmap
			SDL_Surface *pointerSurface = SDL_LoadBMP(fileName);
			if (pointerSurface == NULL)
			{
				LOG_ERROR("Failed to load texture '%s' (%s).", fileName, SDL_GetError());
				return NULL;
			}
			
			// Set the transparent color as bright green (lime)
			unsigned int transparentColor = SDL_MapRGB(pointerSurface->format, 0x00, 0xFF, 0x00);
			if (SDL_SetColorKey(pointerSurface, SDL_TRUE, transparentColor) != 0)
			{
				LOG_ERROR("Failed to set the surface transparent color (%s).", SDL_GetError());
				return NULL;
			}
			
			// Enable RLE compression if requested to
			if (isRleCompressionEnabled)
			{
				if (SDL_SetSurfaceRLE(pointerSurface, 1) != 0)
				{
					LOG_ERROR("Failed to enable RLE compression (%s).", SDL_GetError());
					return NULL;
				}
			}
			
			// Convert the surface to a texture
			SDL_Texture *pointerTexture = SDL_CreateTextureFromSurface(Renderer::pointerMainRenderer, pointerSurface);
			SDL_FreeSurface(pointerSurface);
			if (pointerTexture == NULL)
			{
				LOG_ERROR("Failed to convert the surface to a texture (%s).", SDL_GetError());
				return NULL;
			}
			
			return pointerTexture;
		}
	
	public:
		/** Create a SDL texture from a picture file.
		 * @param fileName The picture file.
		 * @param isRleCompressionEnabled Set to 1 to enable the RLE compression, resulting in faster rendering on highly transparent pictures.
		 */
		Texture(const char *fileName, int isRleCompressionEnabled)
		{
			unsigned int pixelFormat;
			int access;
			
			// Try to load the texture
			_pointerTexture = _loadFromBitmap(fileName, isRleCompressionEnabled);
			if (_pointerTexture == NULL) exit(-1);
			
			// Cache width and height parameters
			if (SDL_QueryTexture(_pointerTexture, &pixelFormat, &access, &_positionRectangle.w, &_positionRectangle.h) != 0)
			{
				LOG_ERROR("Failed to query texture information (%s).", SDL_GetError());
				exit(-1);
			}
		}
		
		/** Free all allocated resources. */
		virtual ~Texture()
		{
			SDL_DestroyTexture(_pointerTexture);
		}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 */
		void render(int x, int y)
		{
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			SDL_RenderCopy(Renderer::pointerMainRenderer, _pointerTexture, NULL, &_positionRectangle);
		}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle. Texture is rotated the specified amount of degrees using its center as rotation point.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 * @param rotationAngle Amount of degrees to rotate the texture around its center.
		 */
		void render(int x, int y, double rotationAngle)
		{
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			SDL_RenderCopyEx(Renderer::pointerMainRenderer, _pointerTexture, NULL, &_positionRectangle, rotationAngle, NULL, SDL_FLIP_NONE);
		}
		
		/** Get the texture width in pixels.
		 * @return The texture width.
		 */
		inline int getWidth()
		{
			return _positionRectangle.w;
		}
		
		/** Get the texture height in pixels.
		 * @return The texture height.
		 */
		inline int getHeight()
		{
			return _positionRectangle.h;
		}
};

#endif