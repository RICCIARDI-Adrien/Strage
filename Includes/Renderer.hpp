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
// Types
//-------------------------------------------------------------------------------------------------
/** All available text rendering colors. */
typedef enum
{
	TEXT_COLOR_ID_BLACK,
	TEXT_COLOR_ID_BLUE,
	TEXT_COLOR_ID_GREEN,
	TEXT_COLOR_ID_RED,
	TEXT_COLOR_ID_DARK_GREEN,
	TEXT_COLOR_IDS_COUNT
} TextColorId;

/** All available font sizes. */
typedef enum
{
	FONT_SIZE_ID_SMALL,
	FONT_SIZE_ID_BIG,
	FONT_SIZE_IDS_COUNT
}FontSizeId;

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

/** End the rendering step by displaying the final picture on the screen. */
void endRendering();

/** Tell if the object located at the provided position can be displayed or not (an object is not displayed if it is out of the screen).
 * @param pointerObjectPositionRectangle The object position on the map.
 * @return 0 if the object is out of the screen,
 * @return 1 if the object can be rendered.
 */
int isDisplayable(SDL_Rect *pointerObjectPositionRectangle);

/** Render the provided text on a texture.
 * @param pointerText The string to render.
 * @param colorId The text color.
 * @param fontSizeId The font size.
 * @return A texture containing the rendered text. Caller must free this texture.
 * @warning This function halts the game if something goes wrong.
 */
SDL_Texture *renderTextToTexture(const char *pointerText, TextColorId colorId, FontSizeId fontSizeId);

/** Display the provided texture at the specified display position.
 * @param pointerTexture The texture to display.
 * @param x Horizontal coordinate of the texture top left corner.
 * @param y Vertical coordinate of the texture top left corner.
 */
void renderTexture(SDL_Texture *pointerTexture, int x, int y);

/** Display the provided texture at the display vertical and horizontal center.
 * @param pointerTexture The texture to display.
 */
void renderCenteredTexture(SDL_Texture *pointerTexture);

}

#endif
