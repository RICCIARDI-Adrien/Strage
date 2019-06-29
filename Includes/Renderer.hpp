/** @file Renderer.hpp
 * The game window renderer.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_RENDERER_HPP
#define HPP_RENDERER_HPP

#include <SDL2/SDL.h>

namespace Renderer
{
	/** All available text rendering colors. */
	typedef enum
	{
		TEXT_COLOR_ID_BLACK,
		TEXT_COLOR_ID_BLUE,
		TEXT_COLOR_ID_LIGHT_BLUE,
		TEXT_COLOR_ID_DARK_GREEN,
		TEXT_COLOR_ID_GREEN,
		TEXT_COLOR_ID_RED,
		TEXT_COLOR_LIGHT_GREY,
		TEXT_COLOR_DARK_GREY,
		TEXT_COLOR_IDS_COUNT
	} TextColorId;

	/** All available font sizes. */
	typedef enum
	{
		FONT_SIZE_ID_SMALL,
		FONT_SIZE_ID_BIG,
		FONT_SIZE_IDS_COUNT
	} FontSizeId;

	/** The renderer used to render to the game window. */
	extern SDL_Renderer *pointerRenderer;

	/** Contains the visible display (it can be though as the camera) leftmost coordinate. */
	extern int displayX;
	/** Contains the visible display (it can be though as the camera) topmost coordinate. */
	extern int displayY;

	/** The display width in pixels. */
	extern int displayWidth;
	/** The display height in pixels. */
	extern int displayHeight;

	/** Initialize the SDL2 library and create a window with a renderer.
	* @param isFullScreenEnabled Set to 1 to start the game in full screen, set to 0 to start it in windowed mode.
	* @return 0 if all went successful,
	* @return -1 if an error occurred.
	*/
	int initialize(bool isFullScreenEnabled);

	/** Free all allocated resources.
	* @note This function must be called after all SDL resources have been freed.
	*/
	void uninitialize();

	/** This function must be called at the really beginning of a frame rendering. */
	void beginFrame();

	/** End the rendering step by displaying the final picture on the screen. */
	void endFrame();

	/** Render the provided text on a texture.
	* @param pointerStringText The string to render.
	* @param colorId The text color.
	* @param fontSizeId The font size.
	* @return A texture containing the rendered text. Caller must free this texture.
	* @warning This function halts the game if something goes wrong.
	*/
	SDL_Texture *renderTextToTexture(const char *pointerStringText, TextColorId colorId, FontSizeId fontSizeId);

	/** Display the provided texture at the specified display position.
	* @param pointerTexture The texture to display.
	* @param x Horizontal coordinate of the texture top left corner.
	* @param y Vertical coordinate of the texture top left corner.
	*/
	void renderTexture(SDL_Texture *pointerTexture, int x, int y);

	/** Display the provided texture at the display vertical and horizontal center.
	* @param pointerTexture The texture to display.
	* @param y Vertical coordinate of the texture top left corner. If set to -1 the texture will be also centered vertically.
	*/
	void renderCenteredTexture(SDL_Texture *pointerTexture, int y = -1);
}

#endif
