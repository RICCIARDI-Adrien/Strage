#ifndef HPP_GUI_LABEL_HPP
#define HPP_GUI_LABEL_HPP

#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

/** @class GuiLabel
 * A text label rendered to a displayable texture.
 * @author Adrien RICCIARDI
 */
class GuiLabel
{
	/** The cached texture. */
	SDL_Texture *_pointerTexture;
	
	/** The rectangle defining the texture size and position on the display. */
	SDL_Rect _positionRectangle;
	
	public:
		/** Create a new label.
		 * @param pointerText The label text.
		 * @param color The color to render the text with.
		 */
		GuiLabel(const char *pointerText, Renderer::TextColorId color)
		{
			// Cache the texture
			_pointerTexture = Renderer::renderTextToTexture(pointerLabel, color);
			
			// Get texture dimensions
			if (SDL_QueryTexture(_pointerTexture, NULL, NULL, &_positionRectangle.w, &_positionRectangle.h)
			{
				LOG_ERROR("failed to render the following label : %s", pointerText);
				exit(-1);
			}
		}
		
		/** Free allocated resources. */
		virtual ~GuiLabel()
		{
			SDL_DestroyTexture(_pointerTexture);
		}
		
		/** Get the label texture.
		 * @return The texture.
		 */
		inline SDL_Texture *getTexture()
		{
			return _pointerTexture;
		}
		
		/** Get the position rectangle.
		 * @return The position rectangle.
		 */
		inline SDL_Rect *getPositionRectangle()
		{
			return &_positionRectangle;
		}
};

#endif