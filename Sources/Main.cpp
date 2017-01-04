/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically free allocated resources on program shutdown. */
static void exitFreeResources()
{
	TextureManager::uninitialize();
	Renderer::uninitialize(); // Must be called at the end because it stops SDL
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	SDL_Event event;
	
	// Engine initialization
	if (Renderer::initialize() != 0) return -1;
	if (TextureManager::initialize() != 0) return -1;
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
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
	exitFreeResources();
	return 0;
}