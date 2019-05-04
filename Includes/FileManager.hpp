/** @file FileManager.hpp
 * Retrieve a file path on all supported platforms (paths are dynamic when running from a macOS bundle).
 * @author Adrien RICCIARDI
 */
#ifndef HPP_FILE_MANAGER_HPP
#define HPP_FILE_MANAGER_HPP

namespace FileManager
{
	/** Retrieve the game base directory.
	* @return 0 on success,
	* @return -1 if an error occurred.
	*/
	int initialize();

	/** Build an absolute path leading to the provided file (only on platform that use dynamic paths, on other platforms the provided file name is returned as-is because relative paths work).
	* @param pointerStringFileName The file to get absolute path. The file can be preceded by directories.
	* @return A static string containing the file absolute path.
	*/
	const char *getFilePath(const char *pointerStringFileName);
}

#endif
