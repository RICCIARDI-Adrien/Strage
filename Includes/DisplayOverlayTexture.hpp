/** @file DisplayOverlayTexture.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_DISPLAY_OVERLAY_TEXTURE_HPP
#define HPP_DISPLAY_OVERLAY_TEXTURE_HPP

#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>

/** @class DisplayOverlayTexture
 * Display a semi transparent texture on the whole display.
 */
class DisplayOverlayTexture: public Texture
{
	public:
		/** Create a screen-sized texture.
		 * @param pointerSDLTexture The SDL texture to display.
		 */
		DisplayOverlayTexture(SDL_Texture *pointerSDLTexture): Texture(pointerSDLTexture)
		{
			if (SDL_SetTextureAlphaMod(_pointerSDLTexture, 96) != 0)
			{
				LOG_ERROR("Failed to set alpha channel (%s).", SDL_GetError());
				exit(-1);
			}
		}
		
		/** Free allocated resources. */
		virtual ~DisplayOverlayTexture() {}
		
		/** Display the stretched texture on the whole screen. */
		virtual void render()
		{
			SDL_RenderCopy(Renderer::pointerRenderer, _pointerSDLTexture, NULL, NULL);
		}
};

#endif
