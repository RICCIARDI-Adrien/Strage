/** @file ControlManager.cpp
 * See ControlManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.hpp>
#include <ControlManager.hpp>
#include <Log.hpp>
#include <SDL2/SDL.h>

namespace ControlManager
{
	/** Hold all keys state. */
	static bool _isKeyPressed[KEY_IDS_COUNT] = { false };
	/** Tell if the last pressed direction key was on the vertical or the horizontal axis. */
	static bool _isLastPressedDirectionKeyOnVerticalAxis = false;

	/** The detected game controller. */
	static SDL_GameController *_pointerGameController = NULL;

	/** What to do when the "go up" key is pressed. */
	static void _handleGoUpKeyPress()
	{
		_isKeyPressed[KEY_ID_GO_UP] = true;
		_isKeyPressed[KEY_ID_GO_DOWN] = false;
		_isLastPressedDirectionKeyOnVerticalAxis = true;
	}

	/** What to do when the "go down" key is pressed. */
	static void _handleGoDownKeyPress()
	{
		_isKeyPressed[KEY_ID_GO_UP] = false;
		_isKeyPressed[KEY_ID_GO_DOWN] = true;
		_isLastPressedDirectionKeyOnVerticalAxis = true;
	}

	/** What to do when the "go left" key is pressed. */
	static void _handleGoLeftKeyPress()
	{
		_isKeyPressed[KEY_ID_GO_LEFT] = true;
		_isKeyPressed[KEY_ID_GO_RIGHT] = false;
		_isLastPressedDirectionKeyOnVerticalAxis = false;
	}

	/** What to do when the "go right" key is pressed. */
	static void _handleGoRightKeyPress()
	{
		_isKeyPressed[KEY_ID_GO_LEFT] = false;
		_isKeyPressed[KEY_ID_GO_RIGHT] = true;
		_isLastPressedDirectionKeyOnVerticalAxis = false;
	}

	int initialize(void)
	{
		// Try to open the first detected game controller
		int Detected_Joysticks_Count = SDL_NumJoysticks();
		for (int i = 0; i < Detected_Joysticks_Count; i++)
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

	bool isKeyPressed(KeyId keyId)
	{
		// Tell that a non-existing key is released
		if (keyId >= KEY_IDS_COUNT) return false;
		
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
				case SDL_SCANCODE_W:
					_handleGoUpKeyPress();
					break;
					
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					_handleGoDownKeyPress();
					break;
					
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					_handleGoLeftKeyPress();
					break;
					
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					_handleGoRightKeyPress();
					break;
					
				case SDL_SCANCODE_SPACE:
					_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = true;
					break;
					
				case SDL_SCANCODE_LCTRL:
					_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = true;
					break;
					
				case SDL_SCANCODE_ESCAPE:
					_isKeyPressed[KEY_ID_PAUSE_GAME] = true;
					break;
					
				case SDL_SCANCODE_RETURN:
					_isKeyPressed[KEY_ID_MENU_SELECT] = true;
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
				case SDL_SCANCODE_W:
					_isKeyPressed[KEY_ID_GO_UP] = false;
					_isLastPressedDirectionKeyOnVerticalAxis = false;
					break;
					
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					_isKeyPressed[KEY_ID_GO_DOWN] = false;
					_isLastPressedDirectionKeyOnVerticalAxis = false;
					break;
					
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					_isKeyPressed[KEY_ID_GO_LEFT] = false;
					_isLastPressedDirectionKeyOnVerticalAxis = true;
					break;
					
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					_isKeyPressed[KEY_ID_GO_RIGHT] = false;
					_isLastPressedDirectionKeyOnVerticalAxis = true;
					break;
					
				case SDL_SCANCODE_SPACE:
					_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = false;
					break;
					
				case SDL_SCANCODE_LCTRL:
					_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = false;
					break;
				
				case SDL_SCANCODE_ESCAPE:
					_isKeyPressed[KEY_ID_PAUSE_GAME] = false;
					break;
					
				case SDL_SCANCODE_RETURN:
					_isKeyPressed[KEY_ID_MENU_SELECT] = false;
					break;
					
				default:
					break;
			}
		}
	}

	void handleGameControllerEvent(SDL_Event *pointerEvent)
	{
		// Analog controls
		if (pointerEvent->type == SDL_CONTROLLERAXISMOTION)
		{
			// Cache values
			unsigned char axis = pointerEvent->caxis.axis;
			short value = pointerEvent->caxis.value;
			
			// Left stick X axis
			if ((axis == SDL_CONTROLLER_AXIS_LEFTX) || (axis == SDL_CONTROLLER_AXIS_RIGHTX))
			{
				if (value <= -CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoLeftKeyPress();
				else if (value >= CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoRightKeyPress();
				else
				{
					_isKeyPressed[KEY_ID_GO_LEFT] = false;
					_isKeyPressed[KEY_ID_GO_RIGHT] = false;
					_isLastPressedDirectionKeyOnVerticalAxis = true;
				}
			}
			
			// Left stick Y axis
			if ((axis == SDL_CONTROLLER_AXIS_LEFTY) || (axis == SDL_CONTROLLER_AXIS_RIGHTY))
			{
				if (value <= -CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoUpKeyPress();
				else if (value >= CONFIGURATION_GAMEPLAY_JOYSTICK_ANALOG_THRESHOLD) _handleGoDownKeyPress();
				else
				{
					_isKeyPressed[KEY_ID_GO_UP] = false;
					_isKeyPressed[KEY_ID_GO_DOWN] = false;
					_isLastPressedDirectionKeyOnVerticalAxis = false;
				}
			}
		}
		// Button press events
		else if (pointerEvent->type == SDL_CONTROLLERBUTTONDOWN)
		{
			switch (pointerEvent->cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_A:
					_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = true;
					break;
					
				case SDL_CONTROLLER_BUTTON_B:
					_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = true;
					break;
					
				case SDL_CONTROLLER_BUTTON_START:
					_isKeyPressed[KEY_ID_PAUSE_GAME] = true;
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
					_isKeyPressed[KEY_ID_PRIMARY_SHOOT] = false;
					break;
					
				case SDL_CONTROLLER_BUTTON_B:
					_isKeyPressed[KEY_ID_SECONDARY_SHOOT] = false;
					break;
					
				case SDL_CONTROLLER_BUTTON_START:
					_isKeyPressed[KEY_ID_PAUSE_GAME] = false;
					break;
					
				default:
					break;
			}
		}
	}

	bool isLastPressedDirectionKeyOnVerticalAxis()
	{
		return _isLastPressedDirectionKeyOnVerticalAxis;
	}
}
