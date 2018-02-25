/** @file Menu.hpp
 * Allow to display a simple items-based menu and to handle items selection.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MENU_HPP
#define HPP_MENU_HPP

namespace Menu
{

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display a centered menu on the whole rendering area.
 * @param stringMenuTitle The menu title, it will be located on the rendering area top and centered.
 * @param stringMenuItemsTexts All menu items to display.
 * @param menuItemsCount How many menu items to display.
 * @return -1 if the user pressed Alt+F4 or closed the window,
 * @return 0 or a positive number representing the index of the selected menu item. Index are in the stringMenuItemsTexts parameter order.
 * @note As strings' coordinates are computed each time this function is called, the function can be used as-is in an in-game window resizing engine.
 */
int display(const char *stringMenuTitle, const char *stringMenuItemsTexts[], int menuItemsCount);

}

#endif
