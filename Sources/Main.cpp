/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <SDL2/SDL.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The game window. */
static SDL_Window *Pointer_Main_Window;

/** The renderer used to render in the game window. */
static SDL_Renderer *Pointer_Main_Renderer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically close SDL resources on program shutdown. */
static void exitUninitializeSdl()
{
	SDL_DestroyWindow(Pointer_Main_Window);
	SDL_Quit();
}

/** Initialize the SDL2 library and create a window with a renderer.
 * @return 0 if all went successful,
 * @return 1 if an error occurred.
 */
static int initializeSdl()
{
	// Initialize the needed subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		LOG("Error : SDL_Init() failed (%s).\n", SDL_GetError());
		goto Exit_Error;
	}
	
	// Create the game window (fullscreen mode)
	Pointer_Main_Window = SDL_CreateWindow("Strage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, /*SDL_WINDOW_FULLSCREEN |*/ SDL_WINDOW_INPUT_GRABBED); // TODO enable fullscreen
	if (Pointer_Main_Window == NULL)
	{
		LOG("Error : failed to create the main window (%s).\n", SDL_GetError());
		goto Exit_Error_Uninitialize_SDL;
	}
	
	// Create the window renderer
	Pointer_Main_Renderer = SDL_CreateRenderer(Pointer_Main_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Pointer_Main_Renderer == NULL)
	{
		LOG("Error : failed to create the main renderer (%s).\n", SDL_GetError());
		goto Exit_Error_Destroy_Window;
	}
	
	// Automatically dispose of SDL resources on program exit
	atexit(exitUninitializeSdl);
	
	// Everything went fine
	return 0;

Exit_Error_Destroy_Window:
	SDL_DestroyWindow(Pointer_Main_Window);
	
Exit_Error_Uninitialize_SDL:
	SDL_Quit();

Exit_Error:
	return 1;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	initializeSdl();
	
	LOG("Information : game engine successfully initialized.\n");
	return 0;
}