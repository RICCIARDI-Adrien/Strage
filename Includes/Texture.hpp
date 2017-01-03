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
		
		/** Render the texture using the main renderer. */
		//void render();
		
		/** Get the corresponding SDL texture.
		 * @return The texture.
		 */
		SDL_Texture *getTexture()
		{
			return _pointerTexture;
		}
};

#endif