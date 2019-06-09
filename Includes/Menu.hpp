/** @file Menu.hpp
 * Allow to display a simple items-based menu and to handle items selection.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_MENU_HPP
#define HPP_MENU_HPP

namespace Menu
{
	/** Display a centered menu on the whole rendering area.
	* @param pointerStringMenuTitle The menu title, it will be located on the rendering area top and centered.
	* @param pointerStringsMenuItemsTexts All menu items to display.
	* @param menuItemsCount How many menu items to display.
	* @return -1 if the user pressed Alt+F4 or closed the window,
	* @return 0 or a positive number representing the index of the selected menu item. Index are in the pointerStringsMenuItemsTexts parameters order.
	* @note As strings' coordinates are computed each time this function is called, the function can be used as-is in an in-game window resizing engine.
	*/
	int display(const char *pointerStringMenuTitle, const char *pointerStringsMenuItemsTexts[], int menuItemsCount);
	
	/** Display a menu listing all game controls.
	 * @return -1 if the user pressed Alt+F4 or closed the window,
	 * @return 0 if the user exited from the menu.
	 */
	int displayControlsMenu();
}

#endif
