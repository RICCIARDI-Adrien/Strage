/** @file Menu.cpp
 * @see Menu.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <Configuration.hpp>
#include <ControlManager.hpp>
#include <Log.hpp>
#include <Menu.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

namespace Menu
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A menu item to display. */
typedef struct
{
	SDL_Texture *pointerNormalTexture; //!< The texture the string is rendered on when the menu item is not focused.
	SDL_Texture *pointerFocusedTexture; //!< The texture the string is rendered on when the menu item is focused.
	int x; //!< Drawing X coordinate.
	int y; //!< Drawing Y coordinate.
} MenuItem;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All items to display. */
static MenuItem _menuItems[CONFIGURATION_MENU_MAXIMUM_ITEMS_COUNT];
/** How many items to display. */
static int _menuItemsCount;

/** The menu title texture to render. */
static SDL_Texture *_pointerMenuTitleTexture;
/** Horizontal coordinate to render the texture at the screen center. */
static int _menuTitleTextureX;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Render all strings to cached textures to avoid rendering them on each frame.
 * @param pointerStringMenuTitle The menu title.
 * @param pointerStringsMenuItemsTexts The menu items.
 */
static inline void _initialize(const char *pointerStringMenuTitle, const char *pointerStringsMenuItemsTexts[])
{
	int i, textureWidth, textureHeight, stringTexturesVerticalHeight, firstStringTextureY;
	
	// Make sure there not too many items
	assert(_menuItemsCount <= CONFIGURATION_MENU_MAXIMUM_ITEMS_COUNT);
	
	// Handle title texture separately because it is not a menu item (it can't be selected or focused)
	// Render texture
	_pointerMenuTitleTexture = Renderer::renderTextToTexture(pointerStringMenuTitle, Renderer::TEXT_COLOR_ID_BLACK, Renderer::FONT_SIZE_ID_BIG);
	// Get texture size
	if (SDL_QueryTexture(_pointerMenuTitleTexture, NULL, NULL, &textureWidth, &textureHeight) != 0)
	{
		LOG_ERROR("Failed to query menu title texture information for texture (%s).", SDL_GetError());
		exit(-1);
	}
	// Compute X coordinate to center the texture
	_menuTitleTextureX = (Renderer::displayWidth - textureWidth) / 2;
	
	// Handle menu items
	for (i = 0; i < _menuItemsCount; i++)
	{
		// Render strings
		_menuItems[i].pointerNormalTexture = Renderer::renderTextToTexture(pointerStringsMenuItemsTexts[i], Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
		_menuItems[i].pointerFocusedTexture = Renderer::renderTextToTexture(pointerStringsMenuItemsTexts[i], Renderer::TEXT_COLOR_ID_DARK_GREEN, Renderer::FONT_SIZE_ID_BIG);
		
		// Compute displaying coordinates
		// Get texture size (texture have same size because only color changes between normal and focused textures, so use the normal one)
		if (SDL_QueryTexture(_menuItems[i].pointerNormalTexture, NULL, NULL, &textureWidth, &textureHeight) != 0)
		{
			LOG_ERROR("Failed to query texture information for texture %d (%s).", i, SDL_GetError());
			exit(-1);
		}
		
		// Compute X coordinate to center the texture
		_menuItems[i].x = (Renderer::displayWidth - textureWidth) / 2;
		
		// Each string is vertically spaced from the other ones by a fixed amount of pixels
		_menuItems[i].y = i * CONFIGURATION_MENU_ITEM_STRINGS_VERTICAL_SPACING;
	}
	
	// Total vertical size of all string textures is the amount of strings * vertical spacing + the last texture height (because it is displayed starting from the last vertical spacing)
	stringTexturesVerticalHeight = (_menuItemsCount * CONFIGURATION_MENU_ITEM_STRINGS_VERTICAL_SPACING) + textureHeight; // The textureHeight variable contains the last texture height
	firstStringTextureY = (Renderer::displayHeight - stringTexturesVerticalHeight) / 2;

	// Add Y offset to all texture coordinates, preserving the yet existing spacing
	for (i = 0; i < _menuItemsCount; i++) _menuItems[i].y += firstStringTextureY;
}

/** Free all textures. */
static inline void _uninitialize()
{
	int i;
	
	// Free all textures
	SDL_DestroyTexture(_pointerMenuTitleTexture);
	for (i = 0; i < _menuItemsCount; i++)
	{
		SDL_DestroyTexture(_menuItems[i].pointerNormalTexture);
		SDL_DestroyTexture(_menuItems[i].pointerFocusedTexture);
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int display(const char *pointerStringMenuTitle, const char *pointerStringsMenuItemsTexts[], int menuItemsCount)
{
	SDL_Event event;
	int returnValue, i, focusedMenuItemIndex = 0, isGoUpKeyPressed = 0, isGoDownKeyPressed = 0, isShootKeyPressed = 0;
	unsigned int frameStartingTime, frameElapsedTime;
	SDL_Texture *pointerTexture;
	
	// This variable is shared by all internal functions, it must be initialized before calling any of the functions
	_menuItemsCount = menuItemsCount;
	
	// Cache textures to display
	_initialize(pointerStringMenuTitle, pointerStringsMenuItemsTexts);
	
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
					returnValue = -1;
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
				if (focusedMenuItemIndex < _menuItemsCount - 1) focusedMenuItemIndex++;
				isGoDownKeyPressed = 1;
			}
		}
		else isGoDownKeyPressed = 0;
		// Shoot key
		if (ControlManager::isKeyPressed(ControlManager::KEY_ID_PRIMARY_SHOOT)) isShootKeyPressed = 1; // Wait for the key to be released to execute the associated action, so the shoot key is not pressed when entering the game (this avoids the player immediately shooting when entering the game because the shoot key is pressed yet)
		else if (isShootKeyPressed)
		{
			returnValue = focusedMenuItemIndex;
			LOG_DEBUG("Selected item %d.", focusedMenuItemIndex);
			goto Exit;
		}
		
		// Display menu
		Renderer::beginRendering(0, 0);
		// Display stretched background (so it can fit any screen resolution)
		SDL_RenderCopy(Renderer::pointerRenderer, TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MENU_BACKGROUND)->getTexture(), NULL, NULL);
		// Display title
		Renderer::renderTexture(_pointerMenuTitleTexture, _menuTitleTextureX, CONFIGURATION_MENU_TITLE_Y);
		// Display items
		for (i = 0; i < _menuItemsCount; i++)
		{
			// Select the right texture according to the focus state
			if (i == focusedMenuItemIndex) pointerTexture = _menuItems[i].pointerFocusedTexture;
			else pointerTexture = _menuItems[i].pointerNormalTexture;
			
			Renderer::renderTexture(pointerTexture, _menuItems[i].x, _menuItems[i].y);
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
