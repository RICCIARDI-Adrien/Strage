/** @file ControlManager.cpp
 * @see ControlManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.hpp>
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

/** The detected game controller. */
static SDL_GameController *_pointerGameController = NULL;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** What to do when the "go up" key is pressed. */
static void _handleGoUpKeyPress()
{
	_isKeyPressed[KEY_ID_GO_UP] = 1;
	_isKeyPressed[KEY_ID_GO_DOWN] = 0;
	_isLastPressedDirectionKeyOnVerticalAxis = 1;
}

/** What to do when the "go down" key is pressed. */
static void _handleGoDownKeyPress()
{
	_isKeyPressed[KEY_ID_GO_UP] = 0;
	_isKeyPressed[KEY_ID_GO_DOWN] = 1;
	_isLastPressedDirectionKeyOnVerticalAxis = 1;
}

/** What to do when the "go left" key is pressed. */
static void _handleGoLeftKeyPress()
{
	_isKeyPressed[KEY_ID_GO_LEFT] = 1;
	_isKeyPressed[KEY_ID_GO_RIGHT] = 0;
	_isLastPressedDirectionKeyOnVerticalAxis = 0;
}

/** What to do when the "go right" key is pressed. */
static void _handleGoRightKeyPress()
{
	_isKeyPressed[KEY_ID_GO_LEFT] = 0;
	_isKeyPressed[KEY_ID_GO_RIGHT] = 1;
	_isLastPressedDirectionKeyOnVerticalAxis = 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize(void)
{
	int i, Detected_Joysticks_Count;
	
	// Try to open the first detected game controller
	Detected_Joysticks_Count = SDL_NumJoysticks();
	for (i = 0; i < Detected_Joysticks_Count; i++)
	{
		// Make sure device is supported by SDL game controller API
		if (!SDL_IsGameController(i)) continue;
	
		// Gain access to controller
		_pointerGameController = SDL_GameControllerOpen(i);
		if (_pointerGameController == NULL) LOG_ERROR("Failed to open game controller with ID %d (%s).", i, SDL_GetError());
		else
		{
			LOG_INFORMATION("Successfully detected game controller '%s'.", SDL_GameControllerName(_pointerGameController));
			break;
		}
	}
	
	return 0;
}

void uninitialize(void)
{
	// Close the opened game controller
	if ((_pointerGameController != NULL) && (SDL_GameControllerGetAttached(_pointerGameController))) SDL_GameControllerClose(_pointerGameController);
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
				_handleGoUpKeyPress();
				break;
				
			case SDL_SCANCODE_DOWN:
				_handleGoDownKeyPress();
				break;
				
			case SDL_SCANCODE_LEFT:
				_handleGoLeftKeyPress();
				break;
				
			case SDL_SCANCODE_RIGHT:
				_handleGoRightKeyPress();
				break;
				
			case SDL_SCANCODE_SPACE:
				_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = 1;
				break;
				
			case SDL_SCANCODE_LCTRL:
				_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = 1;
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
				_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = 0;
				break;
				
			case SDL_SCANCODE_LCTRL:
				_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = 0;
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

void handleGameControllerEvent(SDL_Event *pointerEvent)
{
	unsigned char axis;
	short value;
	
	// Analog controls
	if (pointerEvent->type == SDL_CONTROLLERAXISMOTION)
	{
		// Cache values
		axis = pointerEvent->caxis.axis;
		value = pointerEvent->caxis.value;
		
		// Left stick X axis
		if ((axis == SDL_CONTROLLER_AXIS_LEFTX) || (axis == SDL_CONTROLLER_AXIS_RIGHTX))
		{
			if (value <= -CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoLeftKeyPress();
			else if (value >= CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoRightKeyPress();
			else
			{
				_isKeyPressed[KEY_ID_GO_LEFT] = 0;
				_isKeyPressed[KEY_ID_GO_RIGHT] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 1;
			}
		}
		
		// Left stick Y axis
		if ((axis == SDL_CONTROLLER_AXIS_LEFTY) || (axis == SDL_CONTROLLER_AXIS_RIGHTY))
		{
			if (value <= -CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoUpKeyPress();
			else if (value >= CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoDownKeyPress();
			else
			{
				_isKeyPressed[KEY_ID_GO_UP] = 0;
				_isKeyPressed[KEY_ID_GO_DOWN] = 0;
				_isLastPressedDirectionKeyOnVerticalAxis = 0;
			}
		}
	}
	// Button press events
	else if (pointerEvent->type == SDL_CONTROLLERBUTTONDOWN)
	{
		switch (pointerEvent->cbutton.button)
		{
			case SDL_CONTROLLER_BUTTON_A:
				_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = 1;
				break;
				
			case SDL_CONTROLLER_BUTTON_B:
				_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = 1;
				break;
				
			case SDL_CONTROLLER_BUTTON_START:
				_isKeyPressed[KEY_ID_PAUSE_GAME] = 1;
				break;
				
			case SDL_CONTROLLER_BUTTON_BACK:
				_isKeyPressed[KEY_ID_RETRY_GAME] = 1;
				break;
				
			default:
				break;
		}
	}
	// Button release events
	else if (pointerEvent->type == SDL_CONTROLLERBUTTONUP)
	{
		switch (pointerEvent->cbutton.button)
		{
			case SDL_CONTROLLER_BUTTON_A:
				_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = 0;
				break;
				
			case SDL_CONTROLLER_BUTTON_B:
				_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = 0;
				break;
				
			case SDL_CONTROLLER_BUTTON_START:
				_isKeyPressed[KEY_ID_PAUSE_GAME] = 0;
				break;
				
			case SDL_CONTROLLER_BUTTON_BACK:
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