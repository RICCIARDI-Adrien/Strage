/** @file Log.hpp
 * Simple logging system writing to the console.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_LOG_HPP
#define HPP_LOG_HPP

#include <Configuration.hpp>
#include <cstdio>

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/** Display a red error message on the console.
 * @param stringMessage The message to display.
 * @note Error logs can't be disabled.
 */
#define LOG_ERROR(stringMessage, ...) Log::appendMessage("ERROR [%s:%s():%d] " stringMessage "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/** @def LOG_INFORMATION() Display a yellow information message on the console.
 * @param stringMessage The message to display.
 * @note Information log are enabled with a log level of 1 or more.
 */
#if CONFIGURATION_LOG_LEVEL >= 1
	#define LOG_INFORMATION(stringMessage, ...) Log::appendMessage("INFORMATION [%s:%s():%d] " stringMessage "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG_INFORMATION(stringMessage, ...) {} // Replace by an empty scope instead of nothing in case the log is the only instruction of an if, which would have an empty body if logs are disabled
#endif

/** @def LOG_DEBUG() Display a green debug message on the console.
 * @param stringMessage The message to display.
 * @note Debug log are enabled with a log level of 2.
 */
#if CONFIGURATION_LOG_LEVEL >= 2
	#define LOG_DEBUG(stringMessage, ...) Log::appendMessage("DEBUG [%s:%s():%d] " stringMessage "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG_DEBUG(stringMessage, ...) {} // Replace by an empty scope instead of nothing in case the log is the only instruction of an if, which would have an empty body if logs are disabled
#endif

namespace Log
{
	/** Open log file.
	 * @return -1 if an error occurred,
	 * @return 0 on success.
	 * @note There is no need for an uninitialize() method because the log file is automatically closed on application exit.
	 */
	int initialize();
	
	/** Append a string to the log file.
	 * @param pointerMessageFormat A printf() like message format.
	 */
	void appendMessage(const char *pointerMessageFormat, ...);
}

#endif
