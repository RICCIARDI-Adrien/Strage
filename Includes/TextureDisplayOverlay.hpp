/** @file TextureDisplayOverlay.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_TEXTURE_DISPLAY_OVERLAY_HPP
#define HPP_TEXTURE_DISPLAY_OVERLAY_HPP

#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>

/** @class TextureDisplayOverlay
 * Display a semi transparent texture on the whole display.
 */
class TextureDisplayOverlay: public Texture
{
	public:
		/** Load a bitmap and set alpha channel.
		 * @param fileName The file to load.
		 * @param isRleCompressionEnabled Enable or disable RLE compression (see Texture for more details).
		 */
		TextureDisplayOverlay(const char *fileName, int isRleCompressionEnabled): Texture(fileName, isRleCompressionEnabled)
		{
			if (SDL_SetTextureAlphaMod(_pointerTexture, 96) != 0)
			{
				LOG_ERROR("Failed to set alpha channel (%s).", SDL_GetError());
				exit(-1);
			}
		}
		
		/** Free allocated resources. */
		virtual ~TextureDisplayOverlay() {}
		
		/** Display the stretched texture on the whole screen. */
		void render()
		{
			SDL_RenderCopy(Renderer::pointerRenderer, _pointerTexture, NULL, NULL);
		}
};

#endif
