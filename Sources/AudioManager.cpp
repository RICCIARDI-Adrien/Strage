/** @file AudioManager.cpp
 * @see AudioManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <chrono>
#include <Configuration.hpp>
#include <cstdlib>
#include <Log.hpp>
#include <SDL2/SDL_mixer.h>
#include <thread>

namespace AudioManager
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** How many musics are available. */
#define MUSICS_COUNT (sizeof(_musics) / sizeof(Music))

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Associate a music file name to a playable handle. */
typedef struct
{
	const char *pointerFileName; //!< The file containing the music.
	Mix_Music *pointerMusicHandle; //!< The handle to use to play this music.
} Music;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold all sounds loaded into a chunk. */
static Mix_Chunk *_pointerSounds[SOUND_IDS_COUNT];

/** All available musics. */
static Music _musics[] =
{
	{
		CONFIGURATION_PATH_SOUNDS "/Akashic_Records_-_Epic_Action_Hero.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/Blue_Giraffe_-_Action_Intense_Cinematic.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/Celestial_Aeon_Project_-_Epic.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/E._Erkut_-_Dark_Moment_-_Dark_Epic_Trailer.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/Matti_Paalanen_-_Emotion.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/Matti_Paalanen_-_Epic_Action.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/Soundbay_-_Epic_Future.mp3",
		NULL
	},
	{
		CONFIGURATION_PATH_SOUNDS "/Soundshrim_-_Epic_Adventure.mp3",
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Load a sound from a wave file.
 * @param fileName The file to load.
 * @return A pointer on the chunk containing the sound.
 * @warning This function will stop the program if the sound can't be loaded.
 */
static Mix_Chunk *_loadFromWave(const char *fileName)
{
	Mix_Chunk *pointerChunk;
	
	// Try to load the file
	pointerChunk = Mix_LoadWAV(fileName);
	if (pointerChunk == NULL)
	{
		LOG_ERROR("Failed to load sound file '%s' (%s).", fileName, Mix_GetError());
		exit(-1);
	}
	
	return pointerChunk;
}

/** Make a little pause then play another music randomly chosen. */
static void _playNextMusic()
{
	// Pause 2 seconds to avoid directly starting a different music
	std::this_thread::sleep_for(std::chrono::seconds(2));
	
	playMusic();
}

/** Called when the previous music finished. Call a thread that will start the music out of the callback scope, as requested by SDL Mixer documentation. */
static void _scheduleNextMusic()
{
	static std::thread *pointerMusicPlayerThread = NULL;
	
	// Destroy previous thread if it exists
	if (pointerMusicPlayerThread != NULL)
	{
		pointerMusicPlayerThread->join();
		delete pointerMusicPlayerThread;
	}
	
	// Start a new thread
	pointerMusicPlayerThread = new std::thread(_playNextMusic);
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	unsigned int i;
	
	// Open audio mixer
	if (Mix_OpenAudio(CONFIGURATION_AUDIO_SAMPLING_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) // Chunk size has been randomly chosen due to extremely explicit documentation...
	{
		LOG_ERROR("Failed to open audio device (%s).", Mix_GetError());
		return -1;
	}
	
	// Set the amount of channels (i.e. how many sounds can be played simultaneously)
	Mix_AllocateChannels(CONFIGURATION_AUDIO_CHANNELS_COUNT); // This function can't fail, according to documentation
	
	// Load all sounds
	_pointerSounds[SOUND_ID_AMMUNITION_TAKEN] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Ammunition_Taken.wav");
	_pointerSounds[SOUND_ID_PLAYER_FIRESHOT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Player_Fireshot.wav");
	_pointerSounds[SOUND_ID_PLAYER_HEALED] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Player_Healed.wav");
	_pointerSounds[SOUND_ID_SMALL_ENEMY_FIRESHOT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Small_Enemy_Fireshot.wav");
	_pointerSounds[SOUND_ID_MEDIUM_ENEMY_FIRESHOT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Medium_Enemy_Fireshot.wav");
	_pointerSounds[SOUND_ID_BIG_ENEMY_FIRESHOT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Big_Enemy_Fireshot.wav");
	_pointerSounds[SOUND_ID_ENEMY_BULLET_IMPACT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Bullet_Impact.wav");
	_pointerSounds[SOUND_ID_ENEMY_EXPLOSION] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Explosion.wav");
	_pointerSounds[SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Spawner_Bullet_Impact.wav");
	_pointerSounds[SOUND_ID_ENEMY_SPAWNER_EXPLOSION] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Spawner_Explosion.wav");
	
	// Load all musics
	for (i = 0; i < MUSICS_COUNT; i++)
	{
		// Try to load the file
		_musics[i].pointerMusicHandle = Mix_LoadMUS(_musics[i].pointerFileName);
		if (_musics[i].pointerMusicHandle == NULL)
		{
			LOG_ERROR("Failed to load music '%s' (%s).", _musics[i].pointerFileName, Mix_GetError());
			return -1;
		}
	}
	
	// Call a callback when playing a music has finished
	Mix_HookMusicFinished(_scheduleNextMusic);
	
	return 0;
}

void uninitialize()
{
	unsigned int i;
	
	// Free all musics
	for (i = 0; i < MUSICS_COUNT; i++) Mix_FreeMusic(_musics[i].pointerMusicHandle);
	
	// Free all sounds
	for (i = 0; i < SOUND_IDS_COUNT; i++) Mix_FreeChunk(_pointerSounds[i]);
	
	// Release audio mixer
	Mix_CloseAudio();
}

void playSound(SoundId id)
{
	// Make sure the requested sound exists
	if ((id < 0) || (id >= SOUND_IDS_COUNT))
	{
		LOG_INFORMATION("Unknown sound ID requested (%d).", id);
		return;
	}
	
	// Try to play the sound on the first available channel
	if (Mix_PlayChannel(-1, _pointerSounds[id], 0) == -1) LOG_DEBUG("Failed to play sound ID %d (%s).", id, Mix_GetError());
}

void playMusic()
{
	int musicIndex;
	
	// Select a random music
	musicIndex = rand() % MUSICS_COUNT;
	
	// Try to play it
	if (Mix_PlayMusic(_musics[musicIndex].pointerMusicHandle, 1) != 0) LOG_ERROR("Failed to play music %s (%s).", _musics[musicIndex].pointerFileName, Mix_GetError());
	else LOG_DEBUG("Playing music '%s'.", _musics[musicIndex].pointerFileName);
}

void pauseMusic(int isMusicPaused)
{
	if (isMusicPaused) Mix_PauseMusic();
	else Mix_ResumeMusic();
}

void stopAllSounds()
{
	Mix_HaltChannel(-1);
}

}