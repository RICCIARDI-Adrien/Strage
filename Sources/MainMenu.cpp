/** @file MainMenu.cpp
 * @see MainMenu.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <MainMenu.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

namespace MainMenu
{

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Amount of strings to display. */
#define STRINGS_COUNT 3

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A menu string. */
typedef struct
{
	SDL_Texture *pointerTexture; //!< The texture the string is rendered on.
	int x; //<! Drawing X coordinate.
	int y; //<! Drawing Y coordinate.
} MenuString;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All textures to display. */
static MenuString _menuStrings[STRINGS_COUNT];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Render all strings to cached textures to avoid rendering them on each frame. */
static inline void _initialize()
{
	int i, textureWidth, textureHeight, stringTexturesVerticalHeight, firstStringTextureY;
	
	// Render strings
	_menuStrings[0].pointerTexture = Renderer::renderTextToTexture("[C]ontinue game", Renderer::TEXT_COLOR_ID_BLUE);
	_menuStrings[1].pointerTexture = Renderer::renderTextToTexture("[N]ew game", Renderer::TEXT_COLOR_ID_BLUE);
	_menuStrings[2].pointerTexture = Renderer::renderTextToTexture("[Q]uit", Renderer::TEXT_COLOR_ID_BLUE);
	
	// Compute displaying coordinates
	for (i = 0; i < STRINGS_COUNT; i++)
	{
		// Get texture size
		if (SDL_QueryTexture(_menuStrings[i].pointerTexture, NULL, NULL, &textureWidth, &textureHeight) != 0)
		{
			LOG_ERROR("Failed to query texture information for texture %d (%s).", i, SDL_GetError());
			exit(-1);
		}
		
		// Compute X coordinate to center the texture
		_menuStrings[i].x = (Renderer::displayWidth - textureWidth) / 2;
		
		// Each string is vertically spaced from the other ones by a fixed amount of pixels
		_menuStrings[i].y = i * CONFIGURATION_DISPLAY_MAIN_MENU_STRINGS_VERTICAL_SPACING;
	}
	
	// Total vertical size of all string textures is the amount of strings * vertical spacing + the last texture height (because it is displayed starting from the last vertical spacing)
	stringTexturesVerticalHeight = (STRINGS_COUNT * CONFIGURATION_DISPLAY_MAIN_MENU_STRINGS_VERTICAL_SPACING) + textureHeight; // The textureHeight variable contains the last texture height
	firstStringTextureY = (Renderer::displayHeight - stringTexturesVerticalHeight) / 2;

	// Add Y offset to all texture coordinates, preserving the yet existing spacing
	for (i = 0; i < STRINGS_COUNT; i++) _menuStrings[i].y += firstStringTextureY;
	
	// Set rendering color to gray
	if (SDL_SetRenderDrawColor(Renderer::pointerMainRenderer, 192, 192, 192, 255) != 0)
	{
		LOG_ERROR("Failed to set renderer drawing color (%s).", SDL_GetError());
		exit(-1);
	}
}

/** Free all textures. */
static inline void _uninitialize()
{
	int i;
	
	for (i = 0; i < STRINGS_COUNT; i++) SDL_DestroyTexture(_menuStrings[i].pointerTexture);
	
	// Reset rendering color to black
	if (SDL_SetRenderDrawColor(Renderer::pointerMainRenderer, 0, 0, 0, 255) != 0)
	{
		LOG_ERROR("Failed to reset renderer drawing color (%s).", SDL_GetError());
		exit(-1);
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int display(void)
{
	SDL_Event event;
	int returnValue, i;
	unsigned int frameStartingTime, frameElapsedTime;
	
	// Cache textures to display
	_initialize();
	
	while (1)
	{
		// Store the time when the loop started
		frameStartingTime = SDL_GetTicks();
		
		// Process SDL events
		while (SDL_PollEvent(&event))
		{
			// User closed the game window or pressed alt+F4
			if (event.type == SDL_QUIT)
			{
				returnValue = 2;
				LOG_DEBUG("Quitting game.");
				goto Exit;
			}
			
			// Process key presses
			if (event.type == SDL_KEYDOWN)
			{
				// Use keyboard virtual key code instead of raw scan code to allow whatever language keyboard to be used
				switch (event.key.keysym.sym)
				{
					case SDLK_c:
						returnValue = 0;
						LOG_DEBUG("Continuing game.");
						goto Exit;
						
					case SDLK_n:
						returnValue = 1;
						LOG_DEBUG("New game.");
						goto Exit;
						
					case SDLK_q:
						returnValue = 2;
						LOG_DEBUG("Quitting game.");
						goto Exit;
						
					default:
						break;
				}
			}
		}
		
		// Display menu
		Renderer::beginRendering(0, 0);
		for (i = 0; i < STRINGS_COUNT; i++) Renderer::renderTexture(_menuStrings[i].pointerTexture, _menuStrings[i].x, _menuStrings[i].y);
		Renderer::endRendering();
		
		// Wait enough time to achieve a 60Hz refresh rate
		frameElapsedTime = SDL_GetTicks() - frameStartingTime;
		if (frameElapsedTime < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - frameElapsedTime);
	}
	
Exit:
	// Free resources
	_uninitialize();
	return returnValue;
}

}