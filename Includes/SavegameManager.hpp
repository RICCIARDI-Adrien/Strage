/** @file SavegameManager.hpp
 * Keep player progression across game launches (only integer values can be saved for now).
 * @author Adrien RICCIARDI
 */
#ifndef HPP_SAVEGAME_MANAGER_HPP
#define HPP_SAVEGAME_MANAGER_HPP

namespace SavegameManager
{
	/** All saved items. Don't forget to update _items array default values when adding new members. */
	typedef enum
	{
		SAVEGAME_ITEM_ID_STARTING_LEVEL, //!< The level to load.
		SAVEGAME_ITEM_ID_PLAYER_MAXIMUM_LIFE_POINTS, //!< The maximum life points amount the player managed to have by taking golden medipacks.
		SAVEGAME_ITEM_ID_PLAYER_AMMUNITION, //!< How many ammunition the player has.
		SAVEGAME_ITEM_IDS_COUNT
	} SavegameItemId;

	/** Load the savegame file content.
	* @return -1 if the savegame file is missing or the corrupted,
	* @return 0 if the savegame was successfully loaded.
	*/
	int loadSavegame();

	/** Overwrite the current savegame (if any) or create a new savegame file filled with the current savegame values.
	* @note The function displays error messages if something goes wrong.
	*/
	void storeSavegame();

	/** Get an item value.
	* @param itemId The item identifier.
	* @return The corresponding item value.
	* @note The function crashes the game if a bad item ID is provided.
	*/
	int getSavegameItem(SavegameItemId itemId);

	/** Set an item value.
	* @param itemId The item identifier.
	* @param itemValue The item value.
	* @note The function crashes the game if a bad item ID is provided.
	*/
	void setSavegameItem(SavegameItemId itemId, int itemValue);

	/** Tell whether a savegame file is present.
	* @return false if no savegame file is available,
	* @return true if a savegame file has been found.
	*/
	bool isSavegamePresent();
}

#endif
