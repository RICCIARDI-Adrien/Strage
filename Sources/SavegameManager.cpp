/** @file SavegameManager.cpp
 * See SavegameManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <Configuration.hpp>
#include <cstdio>
#include <FileManager.hpp>
#include <Log.hpp>
#include <SavegameManager.hpp>

namespace SavegameManager
{
	/** All savegame items. */
	static int _items[SAVEGAME_ITEM_IDS_COUNT] = // Set default values to use in case the savegame is not found or when a new game is started
	{
		0,
		CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_LIFE_POINTS_AMOUNT,
		CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_AMMUNITION_AMOUNT
	};

	int loadSavegame()
	{
		// Try to open the file
		FILE *pointerFile = fopen(FileManager::getFilePath(CONFIGURATION_SAVEGAME_FILE_NAME), "r");
		if (pointerFile == NULL)
		{
			LOG_INFORMATION("No savegame file found.");
			return -1;
		}
		
		// Try to load all items
		for (int i = 0; i < SAVEGAME_ITEM_IDS_COUNT; i++)
		{
			// Load the next item
			if (fscanf(pointerFile, "%d", &_items[i]) != 1)
			{
				LOG_INFORMATION("Failed to load item %u. Make sure " CONFIGURATION_SAVEGAME_FILE_NAME " is correct or delete this file to allow the game to run.", i);
				fclose(pointerFile);
				return -1;
			}
			else LOG_DEBUG("Loaded item %u : %d", i, _items[i]);
		}
		
		fclose(pointerFile);
		return 0;
	}

	void storeSavegame()
	{
		// Try to open the file
		FILE *pointerFile = fopen(FileManager::getFilePath(CONFIGURATION_SAVEGAME_FILE_NAME), "w");
		if (pointerFile == NULL)
		{
			LOG_INFORMATION("Could not open savegame file in write mode.");
			return;
		}
		
		// Write all items in order
		for (int i = 0; i < SAVEGAME_ITEM_IDS_COUNT; i++)
		{
			if (fprintf(pointerFile, "%d\n", _items[i]) < 0) LOG_ERROR("Failed to write savegame item %d.", i);
		}
		
		fclose(pointerFile);
	}

	int getSavegameItem(SavegameItemId itemId)
	{
		// Make sure the ID is correct
		assert(itemId < SAVEGAME_ITEM_IDS_COUNT);
		
		return _items[itemId];
	}

	void setSavegameItem(SavegameItemId itemId, int itemValue)
	{
		// Make sure the ID is correct
		assert(itemId < SAVEGAME_ITEM_IDS_COUNT);
		
		_items[itemId] = itemValue;
	}

	bool isSavegamePresent()
	{
		// Try to open the file
		FILE *pointerFile = fopen(FileManager::getFilePath(CONFIGURATION_SAVEGAME_FILE_NAME), "r");
		
		// No file was found
		if (pointerFile == NULL) return false;
		
		// The file is present
		fclose(pointerFile);
		return true;
	}
}
