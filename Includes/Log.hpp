/** @file Log.hpp
 * Simple logging system writing to the console.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_LOG_HPP
#define HPP_LOG_HPP

#include <cstdio>

// TODO namespace ?

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
// Error logs can't be disabled
#define LOG_ERROR(String, ...) printf("\033[31m[%s:%s():%d ERROR] " String "\033[0m", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

// Information log are enabled with a log level of 1 or more
#if CONFIGURATION_LOG_LEVEL >= 1
	#define LOG_INFORMATION(String, ...) printf("\033[33m[%s:%s():%d INFORMATION] " String "\033[0m", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG_INFORMATION(String, ...) {} // Replace by an empty scope instead of nothing in case the log is the only instruction of an if, which would have an empty body if logs are disabled
#endif

// Debug log are enabled with a log level of 2
#if CONFIGURATION_LOG_LEVEL >= 2
	#define LOG_DEBUG(String, ...) printf("\033[32m[%s:%s():%d DEBUG] " String "\033[0m", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG_DEBUG(String, ...) {} // Replace by an empty scope instead of nothing in case the log is the only instruction of an if, which would have an empty body if logs are disabled
#endif

#endif
