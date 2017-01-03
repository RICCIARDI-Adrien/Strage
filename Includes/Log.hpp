/** @file Log.hpp
 * Simple logging system writing to the console.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_LOG_HPP
#define HPP_LOG_HPP

#include <cstdio>

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#if CONFIGURATION_ENABLE_LOGGING
	#define LOG(String, ...) printf("[%s:%d] " String, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG(String, ...)
#endif

#endif
