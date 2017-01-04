/** @file TextureManager.cpp
 * @see TextureManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstdio>
#include <Log.hpp>
#include <Texture.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All tiles. */
static Texture *_pointerTextureManagerTiles[TextureManager::TILE_IDS_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TextureManager::initialize()
{
	int i;
	char filePath[512];
	
	// Load all tiles
	for (i = 0; i < TextureManager::TILE_IDS_COUNT; i++)
	{
		// Create the file path
		snprintf(filePath, sizeof(filePath), CONFIGURATION_PATH_TEXTURES "/%d.bmp", i);
		
		// Try to load the file
		_pointerTextureManagerTiles[i] = new Texture(filePath);
		if (_pointerTextureManagerTiles[i] == NULL)
		{
			LOG("Error : failed to load a tile.\n");
			return -1;
		}
	}
	
	// TODO other
	
	return 0;
}

void TextureManager::uninitialize()
{
	int i;
	
	// Free all tiles
	for (i = 0; i < TextureManager::TILE_IDS_COUNT; i++) delete _pointerTextureManagerTiles[i];
}