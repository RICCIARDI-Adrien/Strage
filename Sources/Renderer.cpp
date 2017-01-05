/** @file Renderer.cpp
 * @see Renderer.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

namespace Renderer
{

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The game window. */
static SDL_Window *_pointerMainWindow;

/** Define the display area. */
static SDL_Rect displayRectangle;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
SDL_Renderer *pointerMainRenderer;

int displayX;
int displayY;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// Initialize the needed subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		LOG_ERROR("SDL_Init() failed (%s).\n", SDL_GetError());
		goto Exit_Error;
	}
	
	// Create the game window (fullscreen mode)
	_pointerMainWindow = SDL_CreateWindow("Strage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, /*SDL_WINDOW_FULLSCREEN*/ 0); // TODO enable fullscreen, TODO runtime configurable resolution
	if (_pointerMainWindow == NULL)
	{
		LOG_ERROR("Failed to create the main window (%s).\n", SDL_GetError());
		goto Exit_Error_Uninitialize_SDL;
	}
	
	// Create the window renderer
	pointerMainRenderer = SDL_CreateRenderer(_pointerMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pointerMainRenderer == NULL)
	{
		LOG_ERROR("Failed to create the main renderer (%s).\n", SDL_GetError());
		goto Exit_Error_Destroy_Window;
	}
	
	displayRectangle.w = CONFIGURATION_DISPLAY_WIDTH;
	displayRectangle.h = CONFIGURATION_DISPLAY_HEIGHT;
	
	// Everything went fine
	return 0;

Exit_Error_Destroy_Window:
	SDL_DestroyWindow(_pointerMainWindow);
	
Exit_Error_Uninitialize_SDL:
	SDL_Quit();

Exit_Error:
	return -1;
}

void uninitialize()
{
	SDL_DestroyRenderer(pointerMainRenderer);
	SDL_DestroyWindow(_pointerMainWindow);
	SDL_Quit();
}

void beginRendering(int x, int y)
{
	// Clean the rendering area
	SDL_RenderClear(pointerMainRenderer);
	
	// Update the display location
	displayRectangle.x = x;
	displayRectangle.y = y;
	displayX = x;
	displayY = y;
}

int isDisplayable(SDL_Rect *pointerObjectPositionRectangle)
{
	return SDL_HasIntersection(&displayRectangle, pointerObjectPositionRectangle);
}

}
