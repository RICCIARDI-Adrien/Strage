/** @file AudioManager.hpp
 * Allow playing and mixing of game sounds and musics.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_AUDIO_MANAGER_HPP
#define HPP_AUDIO_MANAGER_HPP

namespace AudioManager
{

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All available sounds. */
typedef enum
{
	SOUND_ID_AMMUNITION_TAKEN,
	SOUND_ID_PLAYER_FIRESHOT,
	SOUND_ID_PLAYER_HEALED,
	SOUND_ID_ENEMY_SMALL_FIRESHOT,
	SOUND_ID_ENEMY_BULLET_IMPACT,
	SOUND_ID_ENEMY_EXPLOSION,
	SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT,
	SOUND_ID_ENEMY_SPAWNER_EXPLOSION,
	SOUND_IDS_COUNT
} SoundId;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load all game sounds.
 * @return 0 if all files were successfully loaded,
 * @return -1 if an error occurred.
 */
int initialize();

/** Free all allocated resources. */
void uninitialize();

/** Play the specified sound once.
 * @param id Which sound to play.
 * @note Do nothing if the provided ID is bad or if there is not enough resources to play a new sound.
 */
void playSound(SoundId id);

// TODO music

}

#endif