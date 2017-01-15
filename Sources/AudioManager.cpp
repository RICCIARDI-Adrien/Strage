/** @file AudioManager.cpp
 * @see AudioManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <cstdlib>
#include <Log.hpp>
#include <SDL2/SDL_mixer.h>

namespace AudioManager
{

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold all sounds loaded into a chunk. */
static Mix_Chunk *_pointerSounds[SOUND_IDS_COUNT];

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
		LOG_ERROR("Failed to load sound file '%s' (%s).\n", fileName, Mix_GetError());
		exit(-1);
	}
	
	return pointerChunk;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// Open audio mixer
	if (Mix_OpenAudio(CONFIGURATION_AUDIO_SAMPLING_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) // Chunk size has been randomly chosen due to extremely explicit documentation...
	{
		LOG_ERROR("Failed to open audio device (%s).\n", Mix_GetError());
		return -1;
	}
	
	// Set the amount of channels (i.e. how many sounds can be played simultaneously)
	Mix_AllocateChannels(CONFIGURATION_AUDIO_CHANNELS_COUNT); // This function can't fail, according to documentation
	
	// Load all sounds
	_pointerSounds[SOUND_ID_AMMUNITION_TAKEN] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Ammunition_Taken.wav");
	_pointerSounds[SOUND_ID_PLAYER_FIRESHOT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Player_Fireshot.wav");
	_pointerSounds[SOUND_ID_PLAYER_HEALED] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Player_Healed.wav");
	_pointerSounds[SOUND_ID_ENEMY_FIRESHOT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Fireshot.wav");
	_pointerSounds[SOUND_ID_ENEMY_BULLET_IMPACT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Bullet_Impact.wav");
	_pointerSounds[SOUND_ID_ENEMY_EXPLOSION] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Explosion.wav");
	_pointerSounds[SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Spawner_Bullet_Impact.wav");
	_pointerSounds[SOUND_ID_ENEMY_SPAWNER_EXPLOSION] = _loadFromWave(CONFIGURATION_PATH_SOUNDS "/Enemy_Spawner_Explosion.wav");
	
	return 0;
}

void uninitialize()
{
	// Free all sounds
	int i;
	for (i = 0; i < SOUND_IDS_COUNT; i++) Mix_FreeChunk(_pointerSounds[i]);
	
	// Release audio mixer
	Mix_CloseAudio();
}

void playSound(SoundId id)
{
	// Make sure the requested sound exists
	if ((id < 0) || (id >= SOUND_IDS_COUNT))
	{
		LOG_INFORMATION("Unknown sound ID requested (%d).\n", id);
		return;
	}
	
	// Try to play the sound on the first available channel
	if (Mix_PlayChannel(-1, _pointerSounds[id], 0) == -1) LOG_DEBUG("Failed to play sound ID %d (%s).\n", id, Mix_GetError());
}

}