/** @file AudioManager.hpp
 * Allow playing and mixing of game sounds and musics.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_AUDIO_MANAGER_HPP
#define HPP_AUDIO_MANAGER_HPP

namespace AudioManager
{
	/** All available sounds. */
	typedef enum
	{
		SOUND_ID_AMMUNITION_TAKEN,
		SOUND_ID_PLAYER_FIRESHOT,
		SOUND_ID_PLAYER_FIRESHOT_MORTAR_SHELL,
		SOUND_ID_PLAYER_MORTAR_SHELL_RELOADING,
		SOUND_ID_PLAYER_HEALED,
		SOUND_ID_PLAYER_LIFE_INCREASED,
		SOUND_ID_SMALL_ENEMY_FIRESHOT,
		SOUND_ID_MEDIUM_ENEMY_FIRESHOT,
		SOUND_ID_BIG_ENEMY_FIRESHOT,
		SOUND_ID_ENEMY_BULLET_IMPACT,
		SOUND_ID_SMALL_ENEMY_EXPLOSION,
		SOUND_ID_MEDIUM_ENEMY_EXPLOSION,
		SOUND_ID_BIG_ENEMY_EXPLOSION,
		SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT,
		SOUND_ID_ENEMY_SPAWNER_EXPLOSION,
		SOUND_ID_MENU_MOVE,
		SOUND_ID_MENU_SELECT,
		SOUND_ID_MACHINE_GUN_TAKEN,
		SOUND_ID_BULLETPROOF_VEST_TAKEN,
		SOUND_IDS_COUNT,
		SOUND_ID_NO_SOUND //!< Playing this sound ID results in no sound being played
	} SoundId;

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

	/** Start playing randomly all available musics. */
	void playMusic();

	/** Pause or resume music playing.
	* @param isMusicPaused Set to 1 to pause music, set to 0 to resume playing.
	*/
	void pauseMusic(bool isMusicPaused);

	/** Immediately stop all channels. */
	void stopAllSounds();
}

#endif
