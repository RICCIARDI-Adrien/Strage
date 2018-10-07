/** @file HeadUpDisplay.hpp
 * Make the HUD accessible from everywhere in the source code.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_HEAD_UP_DISPLAY_HPP
#define HPP_HEAD_UP_DISPLAY_HPP

namespace HeadUpDisplay
{

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All available mortar states. Using a state identifier instead of a string avoids string duplication and allows to automatically handler text colors. */
typedef enum
{
	MORTAR_STATE_LOW_AMMUNITION,
	MORTAR_STATE_RELOADING,
	MORTAR_STATE_READY,
	MORTAR_STATE_INVALID
} MortarState;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Cache needed textures.
 * @return Always 0 as it can't fail.
 */
int initialize();

/** Update "life points amount" part of the HUD.
 * @param amount How many life points to display.
 */
void setLifePointsAmount(int amount);

/** Update "ammunition amount" part of the HUD.
 * @param amount How many ammunition to display.
 */
void setAmmunitionAmount(int amount);

/** Update "enemies count" part of the HUD.
 * @param amount How many enemies to display.
 */
void setEnemiesAmount(int amount);

/** Update "mortar state" part of the HUD.
 * @param state The new state to display.
 */
void setMortarState(MortarState state);

/** Render the strings with the last provided values to the screen. */
void render();

}

#endif
