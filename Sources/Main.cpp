/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The game window. */
static SDL_Window *pointerMainWindow; // TODO put _ if really private

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
SDL_Renderer *pointerMainRenderer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically free allocated resources on program shutdown. */
static void exitFreeResources()
{
	TextureManager::textureManagerUninitialize();
	
	SDL_DestroyRenderer(pointerMainRenderer);
	SDL_DestroyWindow(pointerMainWindow);
	SDL_Quit();
}

/** Initialize the SDL2 library and create a window with a renderer.
 * @return 0 if all went successful,
 * @return -1 if an error occurred.
 */
static int sdlInitialize()
{
	// Initialize the needed subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		LOG("Error : SDL_Init() failed (%s).\n", SDL_GetError());
		goto Exit_Error;
	}
	
	// Create the game window (fullscreen mode)
	pointerMainWindow = SDL_CreateWindow("Strage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, /*SDL_WINDOW_FULLSCREEN*/ 0); // TODO enable fullscreen
	if (pointerMainWindow == NULL)
	{
		LOG("Error : failed to create the main window (%s).\n", SDL_GetError());
		goto Exit_Error_Uninitialize_SDL;
	}
	
	// Create the window renderer
	pointerMainRenderer = SDL_CreateRenderer(pointerMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pointerMainRenderer == NULL)
	{
		LOG("Error : failed to create the main renderer (%s).\n", SDL_GetError());
		goto Exit_Error_Destroy_Window;
	}
	
	// Everything went fine
	return 0;

Exit_Error_Destroy_Window:
	SDL_DestroyWindow(pointerMainWindow);
	
Exit_Error_Uninitialize_SDL:
	SDL_Quit();

Exit_Error:
	return -1;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	SDL_Event event;
	
	// Engine initialization
	if (sdlInitialize() != 0) return -1; // TODO put in renderer
	if (TextureManager::textureManagerInitialize() != 0) return -1;
	
	// Automatically dispose of allocated resources on program exit
	atexit(exitFreeResources);
	LOG("Information : game engine successfully initialized.\n");
	
	/*Texture t("Textures/0.bmp");
	Texture t2("Textures/Player.bmp");
	
	SDL_RenderClear(pointerMainRenderer);
	SDL_RenderCopy(pointerMainRenderer, t.getTexture(), NULL, NULL);
	SDL_RenderCopy(pointerMainRenderer, t2.getTexture(), NULL, NULL);
	SDL_RenderPresent(pointerMainRenderer);*/
	
	while (1)
	{
		// Handle all events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto Exit;
			}
		}
	}
	
Exit:
	
	
	return 0;
}