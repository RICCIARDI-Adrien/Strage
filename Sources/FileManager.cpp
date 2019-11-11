/** @file FileManager.cpp
 * See FileManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstring>
#include <FileManager.hpp>
#include <Log.hpp>
#include <SDL2/SDL.h>

namespace FileManager
{
	#ifdef CONFIGURATION_BUILD_FOR_MACOS
		/** Game base path (it leads to bundle/Content/Resources on macOS bundle). */
		static char stringBasePath[2048];
	#endif

	int initialize()
	{
		// Only macOS needs that, other platforms use a relative path
		#ifdef CONFIGURATION_BUILD_FOR_MACOS
			char *pointerStringPath;
			unsigned int pathLength;
			
			// Retrieve application path
			pointerStringPath = SDL_GetBasePath();
			if (pointerStringPath == NULL)
			{
				printf("ERROR : failed to obtain the application base path (%s).", SDL_GetError()); // Can't use LOG_ERROR() macro because logging system is not yet initialized (it depends on file manager)
				return -1;
			}
			
			// Make sure it is not too long
			pathLength = strlen(pointerStringPath);
			if (pathLength >= sizeof(stringBasePath))
			{
				printf("ERROR : base path is too long (%u characters).", pathLength); // Can't use LOG_ERROR() macro because logging system is not yet initialized (it depends on file manager)
				SDL_free(pointerStringPath);
				return -1;
			}
			
			// Internally store base path
			strcpy(stringBasePath, pointerStringPath);
			SDL_free(pointerStringPath);
		#endif

		return 0;
	}

	const char *getFilePath(const char *pointerStringFileName)
	{
		#ifdef CONFIGURATION_BUILD_FOR_MACOS
			static char stringPath[16384];
			
			// Build the path
			strcpy(stringPath, stringBasePath); // Base path has always a terminating separator (this is specified by SDL documentation)
			strcat(stringPath, pointerStringFileName);
			return stringPath;
		#else
			return pointerStringFileName;
		#endif
	}
}
