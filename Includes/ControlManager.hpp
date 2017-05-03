/** @file ControlManager.hpp
 * Manage keyboard and game controller (considered as joystick in SDL) inputs.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_CONTROL_MANAGER_HPP
#define HPP_CONTROL_MANAGER_HPP

#include <SDL2/SDL.h>

namespace ControlManager
{

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All handled keys. Keyboard and joystick keys are translated to these virtual key codes. */
typedef enum
{
	KEY_ID_GO_UP,
	KEY_ID_GO_DOWN,
	KEY_ID_GO_LEFT,
	KEY_ID_GO_RIGHT,
	KEY_ID_SHOOT,
	KEY_ID_PAUSE_GAME,
	KEY_ID_RETRY_GAME,
	KEY_IDS_COUNT
} KeyId;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the first joystick. */
int initialize(void);

/** Close any previously opened joystick. */
void uninitialize(void);

/** Tell if a key is pressed or released.
 * @param keyId The key to know state.
 * @return 1 if the key is pressed,
 * @return 0 if the key is released.
 */
int isKeyPressed(KeyId keyId);

void handleKeyboardEvent(SDL_Event *pointerEvent);

//void handleJoystickEvent(SDL_Event *pointerEvent);

/** Tell if the last pressed direction key of the current controller was on the vertical or the horizontal axis.
 * @return 1 if the direction was vertical,
 * @return 0 if the direction was horizontal.
 */
int isLastPressedDirectionKeyOnVerticalAxis();

}

#endif