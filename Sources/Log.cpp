/** @file Log.cpp
 * See Log.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <FileManager.hpp>
#include <Log.hpp>

namespace Log
{
	/** The log file. */
	static FILE *_pointerFile;
	
	/** This is the first registered atexit() callback, so it will called at the really end of the program. */
	static void _exitCloseLogFile()
	{
		fprintf(_pointerFile, "Logging system stopped.\n");
		fclose(_pointerFile);
	}
	
	int initialize()
	{
		// Try to open the file
		_pointerFile = fopen(FileManager::getFilePath(CONFIGURATION_LOG_FILE_NAME), "w");
		if (_pointerFile == NULL)
		{
			printf("Error : failed to open log file '%s' in write-only mode (%s).\n", CONFIGURATION_LOG_FILE_NAME, strerror(errno));
			return -1;
		}
		
		// File was successfully opened, automatically close it on program exit
		atexit(_exitCloseLogFile);
		
		fprintf(_pointerFile, "Logging system initialized.\n");
		return 0;
	}
	
	void appendMessage(const char *pointerMessageFormat, ...)
	{
		// Prepare variable arguments list
		va_list argumentsList;
		va_start(argumentsList, pointerMessageFormat);
		
		// Append the message
		vfprintf(_pointerFile, pointerMessageFormat, argumentsList);
		
		va_end(argumentsList);
	}
}
