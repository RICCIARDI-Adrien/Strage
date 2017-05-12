/** @file FileManager.cpp
 * @see FileManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstring>
#include <FileManager.hpp>
#include <Log.hpp>
#include <SDL2/SDL.h>

namespace FileManager
{

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
#ifdef CONFIGURATION_BUILD_FOR_MACOS
	/** Game base path (it leads to bundle/Content/Resources on macOS bundle). */
	static char basePath[2048];
#endif

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// Only macOS needs that, other platforms use a relative path
	#ifdef CONFIGURATION_BUILD_FOR_MACOS
		char *pointerPath;
		unsigned int pathLength;
		
		// Retrieve application path
		pointerPath = SDL_GetBasePath();
		if (pointerPath == NULL)
		{
			LOG_ERROR("Failed to obtain the application base path (%s).", SDL_GetError());
			return -1;
		}
		
		// Make sure it is not too long
		pathLength = strlen(pointerPath);
		if (pathLength >= sizeof(basePath))
		{
			LOG_ERROR("Base path is too long (%u characters).", pathLength);
			SDL_free(pointerPath);
			return -1;
		}
		
		// Internally store base path
		strcpy(basePath, pointerPath);
		SDL_free(pointerPath);
		LOG_DEBUG("Base path : %s", basePath);
	#endif

	return 0;
}

const char *getFilePath(const char *pointerFileName)
{
	#ifdef CONFIGURATION_BUILD_FOR_MACOS
		static char path[16384];
		
		// Build the path
		strcpy(path, basePath); // Base path has always a terminating separator (this is specified by SDL documentation)
		strcat(path, pointerFileName);
		return path;
	#else
		return pointerFileName;
	#endif
}

}