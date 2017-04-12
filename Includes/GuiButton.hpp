#ifndef HPP_GUI_BUTTON_HPP
#define HPP_GUI_BUTTON_HPP

#include <Renderer.hpp>
#include <SDL2/SDL.h>

/** @class GuiButton
 * A simple button represented by its label.
 * @author Adrien RICCIARDI
 */
class GuiButton // TODO make it inherit from GuiItem if more GUI objects have to be added
{
	private:
		/** The label displayed when the button is not highlighted. */
		GuiLabel *_pointerNormalLabel;
		/** The label displayed when the button is highlighted. */
		GuiLabel *_pointerHighlightedLabel;
		
		/** Tell if the button is currently highlighted or not. */
		int _isHighlighted = 0;
		
	public:
		/** Create a new button.
		 * @param pointerLabelText The button label.
		 */
		GuiButton(const char *pointerLabelText)
		{
			// Render the labels in both colors
			_pointerNormalLabel = new GuiLabel(pointerLabelText, Renderer::TEXT_COLOR_ID_BLUE);
			_pointerNormalLabel = new GuiLabel(pointerLabelText, Renderer::TEXT_COLOR_ID_GREEN);
		}
		
		/** Free allocated resources. */
		virtual ~GuiButton()
		{
			delete _pointerNormalLabel;
			delete _pointerNormalLabel;
		}
		
		/** Tell the button if it is highlighted or not.
		 * @param isHighlighted Set to 1 if the button is highlighted, set to zero if not.
		 */
		inline void setHighlightState(int isHighlighted)
		{
			_isHighlighted = isHighlighted;
		}
		
		//virtual SDL_
		virtual void render
};

#endif