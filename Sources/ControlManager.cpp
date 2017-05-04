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

/** The detected joystick. */
static SDL_Joystick *_pointerJoystick = NULL;

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
	// Try to detect a joystick
	if (SDL_NumJoysticks())
	{
		// One or more joystick is present, open it
		_pointerJoystick = SDL_JoystickOpen(0);
		if (_pointerJoystick == NULL)
		{
			LOG_ERROR("Failed to open the first found joystick (%s).", SDL_GetError());
			return -1;
		}
	}
	
	return 0;
}

void uninitialize(void)
{
	// Close the opened joystick
	if ((_pointerJoystick != NULL) && (SDL_JoystickGetAttached(_pointerJoystick))) SDL_JoystickClose(_pointerJoystick);
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

void handleJoystickEvent(SDL_Event *pointerEvent)
{
	unsigned char button, axis;
	short value;
	
	// Analog controls
	if (pointerEvent->type == SDL_JOYAXISMOTION)
	{
		// Cache values
		axis = pointerEvent->jaxis.axis;
		value = pointerEvent->jaxis.value;
		
		// Left stick X axis
		if (axis == 0)
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
		if (axis == 1)
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
	else if (pointerEvent->type == SDL_JOYBUTTONDOWN)
	{
		// Cache button value
		button = pointerEvent->jbutton.button;
		
		if (button == 0) _isKeyPressed[KEY_ID_SHOOT] = 1;
		else if (button == 7) _isKeyPressed[KEY_ID_PAUSE_GAME] = 1;
		else if (button == 6) _isKeyPressed[KEY_ID_RETRY_GAME] = 1;
	}
	// Button release events
	else if (pointerEvent->type == SDL_JOYBUTTONUP)
	{
		// Cache button value
		button = pointerEvent->jbutton.button;
		
		if (button == 0) _isKeyPressed[KEY_ID_SHOOT] = 0;
		else if (button == 7) _isKeyPressed[KEY_ID_PAUSE_GAME] = 0;
		else if (button == 6) _isKeyPressed[KEY_ID_RETRY_GAME] = 0;
	}
}

int isLastPressedDirectionKeyOnVerticalAxis()
{
	return _isLastPressedDirectionKeyOnVerticalAxis;
}

}