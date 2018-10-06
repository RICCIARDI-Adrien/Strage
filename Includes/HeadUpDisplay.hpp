/** @file HeadUpDisplay.hpp
 * Make the HUD accessible from everywhere in the source code.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_HEAD_UP_DISPLAY_HPP
#define HPP_HEAD_UP_DISPLAY_HPP

namespace HeadUpDisplay
{

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
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

/** Render the strings with the last provided values to the screen. */
void render();

}

#endif
