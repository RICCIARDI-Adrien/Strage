/** @file MainMenu.cpp
 * @see MainMenu.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <ControlManager.hpp>
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
	SDL_Texture *pointerNormalTexture; //!< The texture the string is rendered on when the menu item is not focused.
	SDL_Texture *pointerFocusedTexture; //!< The texture the string is rendered on when the menu item is focused.
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
	_menuStrings[0].pointerNormalTexture = Renderer::renderTextToTexture("Continue game", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	_menuStrings[0].pointerFocusedTexture = Renderer::renderTextToTexture("Continue game", Renderer::TEXT_COLOR_ID_DARK_GREEN, Renderer::FONT_SIZE_ID_BIG);
	_menuStrings[1].pointerNormalTexture = Renderer::renderTextToTexture("New game", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	_menuStrings[1].pointerFocusedTexture = Renderer::renderTextToTexture("New game", Renderer::TEXT_COLOR_ID_DARK_GREEN, Renderer::FONT_SIZE_ID_BIG);
	_menuStrings[2].pointerNormalTexture = Renderer::renderTextToTexture("Quit", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	_menuStrings[2].pointerFocusedTexture = Renderer::renderTextToTexture("Quit", Renderer::TEXT_COLOR_ID_DARK_GREEN, Renderer::FONT_SIZE_ID_BIG);
	
	// Compute displaying coordinates
	for (i = 0; i < STRINGS_COUNT; i++)
	{
		// Get texture size (texture have same size because only color changes between normal and focused textures, so use the normal one)
		if (SDL_QueryTexture(_menuStrings[i].pointerNormalTexture, NULL, NULL, &textureWidth, &textureHeight) != 0)
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
	
	// Free all textures
	for (i = 0; i < STRINGS_COUNT; i++)
	{
		SDL_DestroyTexture(_menuStrings[i].pointerNormalTexture);
		SDL_DestroyTexture(_menuStrings[i].pointerFocusedTexture);
	}
	
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
	int returnValue, i, focusedMenuItemIndex = 0, isGoUpKeyPressed = 0, isGoDownKeyPressed = 0, isShootKeyPressed = 0;
	unsigned int frameStartingTime, frameElapsedTime;
	SDL_Texture *pointerTexture;
	
	// Cache textures to display
	_initialize();
	
	while (1)
	{
		// Store the time when the loop started
		frameStartingTime = SDL_GetTicks();
		
		// Process SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				// User closed the game window or pressed alt+F4
				case SDL_QUIT:
					returnValue = 2;
					LOG_DEBUG("Quitting game.");
					goto Exit;
					
				case SDL_JOYBUTTONUP:
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYAXISMOTION:
					ControlManager::handleJoystickEvent(&event);
					break;
						
				case SDL_KEYUP:
				case SDL_KEYDOWN:
					ControlManager::handleKeyboardEvent(&event);
					break;
			}
		}
		
		// Handle key press
		// Go up key
		if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_UP))
		{
			if (!isGoUpKeyPressed)
			{
				if (focusedMenuItemIndex > 0) focusedMenuItemIndex--;
				isGoUpKeyPressed = 1;
			}
		}
		else isGoUpKeyPressed = 0;
		// Go down key
		if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_DOWN))
		{
			if (!isGoDownKeyPressed)
			{
				if (focusedMenuItemIndex < STRINGS_COUNT - 1) focusedMenuItemIndex++;
				isGoDownKeyPressed = 1;
			}
		}
		else isGoDownKeyPressed = 0;
		// Shoot key
		if (ControlManager::isKeyPressed(ControlManager::KEY_ID_SHOOT)) isShootKeyPressed = 1; // Wait for the key to be released to execute the associated action, so the shoot key is not pressed when entering the game (this avoids the player immediately shooting when entering the game because the shoot key is pressed yet)
		else if (isShootKeyPressed)
		{
			returnValue = focusedMenuItemIndex;
			LOG_DEBUG("Selected item %d.", focusedMenuItemIndex);
			goto Exit;
		}
		
		// Display menu
		Renderer::beginRendering(0, 0);
		for (i = 0; i < STRINGS_COUNT; i++)
		{
			// Select the right texture according to the focus state
			if (i == focusedMenuItemIndex) pointerTexture = _menuStrings[i].pointerFocusedTexture;
			else pointerTexture = _menuStrings[i].pointerNormalTexture;
			
			Renderer::renderTexture(pointerTexture, _menuStrings[i].x, _menuStrings[i].y);
		}
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