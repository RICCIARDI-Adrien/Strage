/** @file Configuration.hpp
 * Gather all program compilation-time configurable parameters.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_CONFIGURATION_HPP
#define HPP_CONFIGURATION_HPP

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// Display
/** Display width in pixels (only in windowed mode). */
#define CONFIGURATION_DISPLAY_WIDTH 1024
/** Display height in pixels (only in windowed mode). */
#define CONFIGURATION_DISPLAY_HEIGHT 768
/** Display refresh period (for a 60Hz refresh frequency, the period is 1/60 = 0.016ms). */
#define CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS 16
/** Compute and display the current frame rate. Displaying requires log levels to be set to 1 or higher. */
#define CONFIGURATION_DISPLAY_IS_FRAME_RATE_DISPLAYING_ENABLED 0
/** HUD life point text screen X coordinate. */
#define CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X 10
/** HUD life point text screen Y coordinate. */
#define CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y 10
/** HUD ammunition text screen X coordinate. */
#define CONFIGURATION_DISPLAY_HUD_AMMUNITION_X 250
/** HUD ammunition text screen Y coordinate. */
#define CONFIGURATION_DISPLAY_HUD_AMMUNITION_Y 10
/** HUD enemies text screen X coordinate. */
#define CONFIGURATION_DISPLAY_HUD_ENEMIES_X 510
/** HUD enemies text screen Y coordinate. */
#define CONFIGURATION_DISPLAY_HUD_ENEMIES_Y 10

// Audio
/** Sampling frequency in Hz. */
#define CONFIGURATION_AUDIO_SAMPLING_FREQUENCY 44100
/** How many sounds can be played simultaneously. */
#define CONFIGURATION_AUDIO_CHANNELS_COUNT 48

// Logs
/** Set which logs to display (error logs are always enabled) :
 * 0 : error messages
 * 1 : error and information messages
 * 2 : error, information and debug messages
 * 3 : all previous stuff plus in-game displaying of some meaningful collision rectangles
 */
#define CONFIGURATION_LOG_LEVEL 1

// Paths
/** Relative path from the executable to the directory containing the levels. */
#define CONFIGURATION_PATH_LEVELS "Levels"
/** Relative path from the executable to the directory containing the sounds. */
#define CONFIGURATION_PATH_SOUNDS "Sounds"
/** Relative path from the executable to the directory containing the textures. */
#define CONFIGURATION_PATH_TEXTURES "Textures"

// Levels
/** Level maximum width in blocks. */
#define CONFIGURATION_LEVEL_MAXIMUM_WIDTH 256
/** Level maximum height in blocks. */
#define CONFIGURATION_LEVEL_MAXIMUM_HEIGHT 256
/** A block size in pixels. */
#define CONFIGURATION_LEVEL_BLOCK_SIZE 64

// Gameplay
/** The probability (in range 0% to 100%) to spawn a medipack when an enemy dies. */
#define CONFIGURATION_GAMEPLAY_MEDIPACK_ITEM_SPAWN_PROBABILITY_PERCENTAGE 10
/** The probability (in range 0% to 100%) to spawn ammunition when an enemy dies. */
#define CONFIGURATION_GAMEPLAY_AMMUNITION_ITEM_SPAWN_PROBABILITY_PERCENTAGE 20
// The probability (in range 0% to 100%) to spawn a specific enemy (sum of all enemies percentages must be 100 and each percentage must be less than the previous one)
/** Small enemy spawning probability. */
#define CONFIGURATION_GAMEPLAY_SMALL_ENEMY_SPAWN_PROBABILITY_PERCENTAGE 80
/** Medium enemy spawning probability. */
#define CONFIGURATION_GAMEPLAY_MEDIUM_ENEMY_SPAWN_PROBABILITY_PERCENTAGE 15
/** Big enemy spawning probability. */
#define CONFIGURATION_GAMEPLAY_BIG_ENEMY_SPAWN_PROBABILITY_PERCENTAGE 5

#endif