/** @file Renderer.hpp
 * The game window renderer.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_RENDERER_HPP
#define HPP_RENDERER_HPP

#include <SDL2/SDL.h>

namespace Renderer
{

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The renderer used to render to the game window. */
extern SDL_Renderer *pointerMainRenderer;

/** On rendering, contains the visible display leftmost coordinate. */
extern int displayX;
/** On rendering, contains the visible display topmost coordinate. */
extern int displayY;

/** The display width in pixels. */
extern int displayWidth;
/** The display height in pixels. */
extern int displayHeight;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the SDL2 library and create a window with a renderer.
 * @param isFullScreenEnabled Set to 1 to start the game in full screen, set to 0 to start it in windowed mode.
 * @return 0 if all went successful,
 * @return -1 if an error occurred.
 */
int initialize(int isFullScreenEnabled);

/** Free all allocated resources.
 * @note This function must be called after all SDL resources have been freed.
 */
void uninitialize();

/** Start rendering from the specified map coordinates.
 * @param x Display left corner will be rendered with the scene stuff located at this horizontal coordinate.
 * @param x Display top corner will be rendered with the scene stuff located at this vertical coordinate.
 */
void beginRendering(int x, int y);

/** Tell if the object located at the provided position can be displayed or not (an object is not displayed if it is out of the screen).
 * @param pointerObjectPositionRectangle The object position on the map.
 * @return 0 if the object is out of the screen,
 * @return 1 if the object can be rendered.
 */
int isDisplayable(SDL_Rect *pointerObjectPositionRectangle);

/** Display the provided text at the specified display position.
 * @param x Horizontal coordinate of the text top left corner.
 * @param y Vertical coordinate of the text top left corner.
 * @param stringText The text to display.
 */
void renderText(int x, int y, const char *stringText);

}

#endif
