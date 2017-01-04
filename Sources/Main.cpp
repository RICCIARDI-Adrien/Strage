/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically free allocated resources on program shutdown. */
static void exitFreeResources()
{
	LevelManager::uninitialize();
	TextureManager::uninitialize();
	Renderer::uninitialize(); // Must be called at the end because it stops SDL
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	SDL_Event event;
	unsigned int Starting_Time, Elapsed_Time;
	
	// TEST
	int camX = 0, camY = 0;
	
	// Engine initialization
	if (Renderer::initialize() != 0) return -1;
	if (TextureManager::initialize() != 0) return -1;
	if (LevelManager::initialize() != 0) return -1;
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
	atexit(exitFreeResources);
	LOG_INFORMATION("Game engine successfully initialized.\n");
	
	// TEST
	LevelManager::loadLevel("Levels/Test_Scene.csv", "Levels/Test_Objects.csv");
	
	while (1)
	{
		// Store the time when the loop started
		Starting_Time = SDL_GetTicks();
		
		// Handle all events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto Exit;
					
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							camY -= 10;
							break;
							
						case SDL_SCANCODE_DOWN:
							camY += 10;
							break;
						
						case SDL_SCANCODE_LEFT:
							camX -= 10;
							break;
						
						case SDL_SCANCODE_RIGHT:
							camX += 10;
							break;
							
						default:
							break;
					}
					break;
				}
			}
		}
		
		// Start rendering
		SDL_RenderClear(Renderer::pointerMainRenderer);
		
		// Render the level walls
		LevelManager::renderScene(camX, camY);
		
		SDL_RenderPresent(Renderer::pointerMainRenderer);
		
		// Wait enough time to achieve a 60Hz refresh rate
		Elapsed_Time = SDL_GetTicks() - Starting_Time;
		if (Elapsed_Time < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - Elapsed_Time);
	}
	
Exit:
	return 0;
}