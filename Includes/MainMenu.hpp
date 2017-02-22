/** @file MainMenu.hpp
 * Display a simple choice menu on game start.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MAIN_MENU_HPP
#define HPP_MAIN_MENU_HPP

namespace MainMenu
{

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display the menu and return the user choice.
 * @return 0 if the user selected "continue game",
 * @return 1 if the user selected "new game",
 * @return 2 if the user selected "quit".
 */
int display(void);

}

#endif