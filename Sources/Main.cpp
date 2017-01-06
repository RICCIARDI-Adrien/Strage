/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <Player.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

// TEST
#include <PickableEntityMedipack.hpp>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All keyboard keys that matter. */
typedef enum
{
	KEYBOARD_KEY_ID_ARROW_UP,
	KEYBOARD_KEY_ID_ARROW_DOWN,
	KEYBOARD_KEY_ID_ARROW_LEFT,
	KEYBOARD_KEY_ID_ARROW_RIGHT,
	KEYBOARD_KEY_IDS_COUNT
} KeyboardKeyId;

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
	int isKeyPressed[KEYBOARD_KEY_IDS_COUNT] = {0};
	
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
	PickableEntityMedipack m1(81 + 64, 93);
	Player player(5 * 64 + 20, 80);
	
	while (1)
	{
		// Store the time when the loop started
		Starting_Time = SDL_GetTicks();
		
		// Handle all relevant events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto Exit;
				
				// Remember which key are pressed
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 1;
							break;
							
						case SDL_SCANCODE_DOWN:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 1;
							break;
							
						case SDL_SCANCODE_LEFT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 1;
							break;
							
						case SDL_SCANCODE_RIGHT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 1;
							break;
							
						default:
							break;
					}
					break;
				}
				
				// Some key has been released
				case SDL_KEYUP:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 0;
							break;
							
						case SDL_SCANCODE_DOWN:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 0;
							break;
							
						case SDL_SCANCODE_LEFT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 0;
							break;
							
						case SDL_SCANCODE_RIGHT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 0;
							break;
							
						default:
							break;
					}
					break;
				}
			}
		}
		
		// React to player key press without depending of keyboard key repetition rate
		if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP]) player.moveToUp();
		else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN]) player.moveToDown();
		// Up/down and left/right keys are separated to allow the player to go in both horizontal and vertical directions in the same time
		if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT]) player.moveToLeft();
		else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT]) player.moveToRight();
		
		camX = player.getX() - (CONFIGURATION_DISPLAY_WIDTH / 2) + 16;
		camY = player.getY() - (CONFIGURATION_DISPLAY_HEIGHT / 2) + 16;
		
		// Start rendering
		Renderer::beginRendering(camX, camY);
		
		// Render the level walls
		LevelManager::renderScene(camX, camY);
		
		// Display pickable entities (as they are laying on the floor, any other entity is walking on top of them)
		// TODO
		
		m1.render();
		
		// TEST
		//player.render(CONFIGURATION_DISPLAY_WIDTH / 2 - 10, CONFIGURATION_DISPLAY_HEIGHT / 2 - 10, 38);
		player.render();
		
		SDL_RenderPresent(Renderer::pointerMainRenderer);
		
		// Wait enough time to achieve a 60Hz refresh rate
		Elapsed_Time = SDL_GetTicks() - Starting_Time;
		if (Elapsed_Time < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - Elapsed_Time);
	}
	
Exit:
	return 0;
}