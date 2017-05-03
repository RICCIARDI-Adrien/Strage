/** @file ControlManager.cpp
 * @see ControlManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <ControlManager.hpp>
#include <Log.hpp>
#include <SDL2/SDL.h>

namespace ControlManager
{

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold all keys state. */
static int _isKeyPressed[KEY_IDS_COUNT] = {0};
/** Tell if the last pressed direction key was on the vertical or the horizontal axis. */
static int _isLastPressedDirectionKeyOnVerticalAxis = 0;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize(void)
{
	// TODO
	
	return 0;
}

void uninitialize(void)
{
	// TODO
}

int isKeyPressed(KeyId keyId)
{
	// Tell that a non-existing key is released
	if (keyId >= KEY_IDS_COUNT) return 0;
	
	return _isKeyPressed[keyId];
}

void handleKeyboardEvent(SDL_Event *pointerEvent)
{
	// Handle key press
	if (pointerEvent->key.state == SDL_PRESSED)
	{
		switch (pointerEvent->key.keysym.scancode)
		{
			case SDL_SCANCODE_UP:
				_isKeyPressed[KEY_ID_GO_UP] = 1;
				_isKeyPressed[KEY_ID_GO_DOWN] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 1;
				break;
				
			case SDL_SCANCODE_DOWN:
				_isKeyPressed[KEY_ID_GO_UP] = 0;
				_isKeyPressed[KEY_ID_GO_DOWN] = 1;
				_isLastPressedDirectionKeyOnVerticalAxis = 1;
				break;
				
			case SDL_SCANCODE_LEFT:
				_isKeyPressed[KEY_ID_GO_LEFT] = 1;
				_isKeyPressed[KEY_ID_GO_RIGHT] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 0;
				break;
				
			case SDL_SCANCODE_RIGHT:
				_isKeyPressed[KEY_ID_GO_LEFT] = 0;
				_isKeyPressed[KEY_ID_GO_RIGHT] = 1;
				_isLastPressedDirectionKeyOnVerticalAxis = 0;
				break;
				
			case SDL_SCANCODE_SPACE:
				_isKeyPressed[KEY_ID_SHOOT] = 1;
				break;
				
			case SDL_SCANCODE_ESCAPE:
				_isKeyPressed[KEY_ID_PAUSE_GAME] = 1;
				break;
			
			case SDL_SCANCODE_R:
				_isKeyPressed[KEY_ID_RETRY_GAME] = 1;
				break;
				
			default:
				break;
		}
	}
	// Handle key release
	else
	{
		switch (pointerEvent->key.keysym.scancode)
		{
			case SDL_SCANCODE_UP:
				_isKeyPressed[KEY_ID_GO_UP] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 0;
				break;
				
			case SDL_SCANCODE_DOWN:
				_isKeyPressed[KEY_ID_GO_DOWN] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 0;
				break;
				
			case SDL_SCANCODE_LEFT:
				_isKeyPressed[KEY_ID_GO_LEFT] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 1;
				break;
				
			case SDL_SCANCODE_RIGHT:
				_isKeyPressed[KEY_ID_GO_RIGHT] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 1;
				break;
				
			case SDL_SCANCODE_SPACE:
				_isKeyPressed[KEY_ID_SHOOT] = 0;
				break;
			
			case SDL_SCANCODE_ESCAPE:
				_isKeyPressed[KEY_ID_PAUSE_GAME] = 0;
				break;
				
			case SDL_SCANCODE_R:
				_isKeyPressed[KEY_ID_RETRY_GAME] = 0;
				break;
				
			default:
				break;
		}
	}
}

int isLastPressedDirectionKeyOnVerticalAxis()
{
	return _isLastPressedDirectionKeyOnVerticalAxis;
}

}