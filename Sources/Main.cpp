/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <Configuration.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <EntityAnimatedTexture.hpp>
#include <EntityAnimatedTextureEnemySpawnerExplosion.hpp>
#include <EntityAnimatedTextureShipExplosion.hpp>
#include <EntityEnemySpawner.hpp>
#include <FightingEntityEnemy.hpp>
#include <FightingEntityPlayer.hpp>
#include <FightingEntityEnemyBig.hpp>
#include <FightingEntityEnemyMedium.hpp>
#include <FightingEntityEnemySmall.hpp>
#include <list>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <MovingEntityBullet.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureDisplayOverlay.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All keyboard keys that matter. */
typedef enum
{
	KEYBOARD_KEY_ID_ARROW_UP,
	KEYBOARD_KEY_ID_ARROW_DOWN,
	KEYBOARD_KEY_ID_ARROW_LEFT,
	KEYBOARD_KEY_ID_ARROW_RIGHT,
	KEYBOARD_KEY_ID_SPACE,
	KEYBOARD_KEY_IDS_COUNT
} KeyboardKeyId;

/** All strings the interface can display. */
typedef enum
{
	INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT,
	INTERFACE_STRING_ID_AMMUNITION_AMOUNT,
	INTERFACE_STRING_ID_ENEMIES_COUNT,
	INTERFACE_STRING_ID_GAME_PAUSED,
	INTERFACE_STRING_ID_GAME_LOST,
	INTERFACE_STRING_ID_GAME_WON,
	INTERFACE_STRING_IDS_COUNT
} InterfaceStringId;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All bullets shot by the player. */
static std::list<MovingEntityBullet *> _playerBulletsList;
/** All bullets shot by the enemies. */
static std::list<MovingEntityBullet *> _enemiesBulletsList;

/** All enemies. */
static std::list<FightingEntityEnemy *> _enemiesList;

/** All animated textures. */
static std::list<EntityAnimatedTexture *> _animatedTexturesList;

/** How many pixels to add to spawn X coordinate to make the enemy spawn on block center. */
static int _enemySpawnOffsetX;
/** How many pixels to add to spawn Y coordinate to make the enemy spawn on block center. */
static int _enemySpawnOffsetY;

/** How many pixels to subtract to the player X coordinate to obtain the scene camera X coordinate. */
static int _cameraOffsetX;
/** How many pixels to subtract to the player Y coordinate to obtain the scene camera Y coordinate. */
static int _cameraOffsetY;

/** When set to 1, stop game updating and display a text saying that the player is dead. */
static int _isPlayerDead = 0;
/** Set to 1 when the player has been hit. */
static int _isPlayerHit = 0;

/** Draw the screen in red if the player has been hit. */
static TextureDisplayOverlay *_pointerPlayerHitOverlayTexture;

/** The current level number. */
static int _currentLevelNumber = 0;

/** When set to 1, stop game updating but continue displaying the scene. */
static int _isGamePaused = 0;
/** Set to 1 when the player has finished all levels. */
static int _isGameFinished = 0;

/** Cache all interface strings (rendered to textures) because they are really slow to render. */
static SDL_Texture *_pointerInterfaceStrings[INTERFACE_STRING_IDS_COUNT];

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
// The variable must be outside of any namespace
FightingEntityPlayer *pointerPlayer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Free all lists content. */
static void _clearAllLists()
{
	std::list<EntityEnemySpawner *>::iterator enemySpawnersListIterator;
	for (enemySpawnersListIterator = LevelManager::enemySpawnersList.begin(); enemySpawnersListIterator != LevelManager::enemySpawnersList.end(); ++enemySpawnersListIterator) delete *enemySpawnersListIterator;
	LevelManager::enemySpawnersList.clear();
	
	std::list<MovingEntityBullet *>::iterator bulletsListIterator;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator) delete *bulletsListIterator;
	_playerBulletsList.clear();
	
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator) delete *bulletsListIterator;
	_enemiesBulletsList.clear();

	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator) delete *enemiesListIterator;
	_enemiesList.clear();
	
	std::list<EntityAnimatedTexture *>::iterator animatedTexturesListIterator;
	for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) delete *animatedTexturesListIterator;
	_animatedTexturesList.clear();
}

/** Automatically free allocated resources on program shutdown. */
static void _exitFreeResources()
{
	int i;
	
	// Free all cached interface strings
	for (i = 0; i < INTERFACE_STRING_IDS_COUNT; i++) SDL_DestroyTexture(_pointerInterfaceStrings[i]);
	
	// Delete all entities
	_clearAllLists();
	
	delete pointerPlayer;
	
	AudioManager::uninitialize();
	LevelManager::uninitialize();
	TextureManager::uninitialize();
	Renderer::uninitialize(); // Must be called at the end because it stops SDL
}

/** Spawn a random item (or nothing) on the block into which coordinates are contained.
 * @param x X coordinate in pixels.
 * @param y Y coordinate in pixels.
 */
static inline void _spawnItem(int x, int y)
{
	int blockContent;
	
	// Spawn nothing if the block contains an item yet
	blockContent = LevelManager::getBlockContent(x, y);
	if ((blockContent & LevelManager::BLOCK_CONTENT_MEDIPACK) || (blockContent & LevelManager::BLOCK_CONTENT_GOLDEN_MEDIPACK) || (blockContent & LevelManager::BLOCK_CONTENT_AMMUNITION)) return;
	
	// Select which item to spawn
	if (rand() % 2 == 0)
	{
		// Spawn a medipack
		if (rand() % 100 < CONFIGURATION_GAMEPLAY_MEDIPACK_ITEM_SPAWN_PROBABILITY_PERCENTAGE)
		{
			// Spawn the item
			blockContent |= LevelManager::BLOCK_CONTENT_MEDIPACK;
			LevelManager::setBlockContent(x, y, blockContent);
			
			LOG_DEBUG("Enemy dropped a medipack.");
		}
	}
	else
	{
		// Spawn ammunition
		if (rand() % 100 < CONFIGURATION_GAMEPLAY_AMMUNITION_ITEM_SPAWN_PROBABILITY_PERCENTAGE)
		{
			// Spawn the item
			blockContent |= LevelManager::BLOCK_CONTENT_AMMUNITION;
			LevelManager::setBlockContent(x, y, blockContent);
			
			LOG_DEBUG("Enemy dropped ammunition.");
		}
	}
}

/** Tell whether an enemy can be spawned on the block located at the provided map coordinates.
 * @param x X map coordinate in the block.
 * @param y Y map coordinate in the block.
 * @return 0 if an enemy can't be spawned on this block,
 * @return 1 if the block is available.
 */
int _isBlockAvailableForSpawn(int x, int y)
{
	int blockContent;
	
	// Get the block content
	blockContent = LevelManager::getBlockContent(x, y);
	
	// No room to spawn an enemy
	if (blockContent & (LevelManager::BLOCK_CONTENT_WALL | LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER | LevelManager::BLOCK_CONTENT_ENEMY)) return 0;
	return 1;
}

/** Try to spawn an enemy around a spawner.
 * @param enemySpawnerX Spawner X map coordinate.
 * @param enemySpawnerY Spawner Y map coordinate.
 * @return NULL if no enemy could be spawned,
 * @return a valid pointer if an enemy was spawned.
 */
static inline FightingEntityEnemy *_spawnEnemy(int enemySpawnerX, int enemySpawnerY)
{
	int x, y, spawningPercentage;
	
	// Find a free block to spawn the enemy onto
	// North-west block
	x = enemySpawnerX - CONFIGURATION_LEVEL_BLOCK_SIZE;
	y = enemySpawnerY - CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// North block
	x = enemySpawnerX;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// North-east block
	x = enemySpawnerX + CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// West block
	x = enemySpawnerX - CONFIGURATION_LEVEL_BLOCK_SIZE;
	y = enemySpawnerY;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// East block
	x = enemySpawnerX + CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// South-west block
	x = enemySpawnerX - CONFIGURATION_LEVEL_BLOCK_SIZE;
	y = enemySpawnerY + CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// South block
	x = enemySpawnerX;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	// South-east block
	x = enemySpawnerX + CONFIGURATION_LEVEL_BLOCK_SIZE;
	if (_isBlockAvailableForSpawn(x, y)) goto Spawn_Enemy;
	
	// No room to spawn an enemy
	return NULL;
	
Spawn_Enemy:
	// Adjust coordinates to spawn the enemy at the block center
	x += _enemySpawnOffsetX;
	y += _enemySpawnOffsetY;
	
	LOG_DEBUG("Spawned an enemy on map coordinates (%d, %d).", x, y);
	
	// Select which enemy to spawn
	spawningPercentage = rand() % 100;
	// Start with the smaller percentage and continue so on
	if (spawningPercentage <= CONFIGURATION_GAMEPLAY_BIG_ENEMY_SPAWN_PROBABILITY_PERCENTAGE) return new FightingEntityEnemyBig(x, y);
	if (spawningPercentage <= CONFIGURATION_GAMEPLAY_MEDIUM_ENEMY_SPAWN_PROBABILITY_PERCENTAGE) return new FightingEntityEnemyMedium(x, y);
	return new FightingEntityEnemySmall(x, y);
}

/** Load next level. */
static inline void _loadNextLevel()
{
	// Are all levels completed ?
	if (_currentLevelNumber == LevelManager::levelsCount)
	{
		_isGameFinished = 1;
		_isGamePaused = 1;
		return;
	}
	
	// Stop currently playing sounds
	AudioManager::stopAllSounds();
	
	// Free all entities
	_clearAllLists();
	
	// Try to load next level
	if (LevelManager::loadLevel(_currentLevelNumber) != 0)
	{
		LOG_ERROR("Failed to load level %d.", _currentLevelNumber);
		exit(-1);
	}
	_currentLevelNumber++;
}

/** Update all game actors. */
static inline void _updateGameLogic()
{
	// Check if pickable objects can be taken by the player or if the level end has been reached
	if (pointerPlayer->update() == 2)
	{
		// Next level can be loaded only if all enemies are dead
		if (_enemiesList.empty())
		{
			// Restore player life
			pointerPlayer->modifyLife(100);
			
			_loadNextLevel();
			return;
		}
	}
	
	// Check if player bullets have hit a wall or an enemy
	std::list<MovingEntityBullet *>::iterator bulletsListIterator = _playerBulletsList.begin();
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	std::list<EntityEnemySpawner *>::iterator enemySpawnersListIterator;
	MovingEntityBullet *pointerPlayerBullet;
	FightingEntityEnemy *pointerEnemy;
	EntityEnemySpawner *pointerEnemySpawner;
	while (bulletsListIterator != _playerBulletsList.end())
	{
		pointerPlayerBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerPlayerBullet->update() != 0)
		{
			delete pointerPlayerBullet;
			bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		// Check with all enemies for a collision
		for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator)
		{
			pointerEnemy = *enemiesListIterator;
			
			// Did the bullet hit this enemy ?
			if (SDL_HasIntersection(pointerPlayerBullet->getPositionRectangle(), pointerEnemy->getPositionRectangle()))
			{
				// Wound the enemy
				pointerEnemy->modifyLife(pointerPlayerBullet->getDamageAmount());
				
				AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_BULLET_IMPACT);
				LOG_DEBUG("Enemy hit.");
				
				// Remove the bullet
				delete pointerPlayerBullet;
				bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
				
				break;
			}
		}
		
		// The bullet collided an enemy and was destroyed, check next bullet
		if (enemiesListIterator != _enemiesList.end()) continue;
		
		// Check with all spawners for a collision
		for (enemySpawnersListIterator = LevelManager::enemySpawnersList.begin(); enemySpawnersListIterator != LevelManager::enemySpawnersList.end(); ++enemySpawnersListIterator)
		{
			pointerEnemySpawner = *enemySpawnersListIterator;
			
			if (SDL_HasIntersection(pointerPlayerBullet->getPositionRectangle(), pointerEnemySpawner->getPositionRectangle()))
			{
				// Damage the enemy spawner
				pointerEnemySpawner->modifyLife(pointerPlayerBullet->getDamageAmount());
				
				AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT);
				LOG_DEBUG("Enemy spawner hit.");
				
				// Remove the bullet
				delete pointerPlayerBullet;
				bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
				
				break;
			}
		}
		
		// The bullet collided an enemy spawner and was destroyed, check next bullet
		if (enemySpawnersListIterator != LevelManager::enemySpawnersList.end()) continue;
		
		// The bullet is still alive, check next one
		++bulletsListIterator;
	}
	
	// Update enemies artificial intelligence
	MovingEntityBullet *pointerBullet;
	int result;
	SDL_Rect *pointerPositionRectangle;
	enemiesListIterator = _enemiesList.begin();
	while (enemiesListIterator != _enemiesList.end())
	{
		pointerEnemy = *enemiesListIterator;
		
		result = pointerEnemy->update();
		// Remove the enemy if it is dead
		if (result == 1)
		{
			// Spawn an explosion effect
			pointerPositionRectangle = pointerEnemy->getPositionRectangle();
			
			_animatedTexturesList.push_front(new EntityAnimatedTextureShipExplosion(pointerPositionRectangle->x, pointerPositionRectangle->y));
			AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_EXPLOSION);
			
			// Spawn an item on the current block if player is lucky
			_spawnItem(pointerPositionRectangle->x + (pointerPositionRectangle->w / 2), pointerPositionRectangle->y + (pointerPositionRectangle->h / 2)); // Use enemy center coordinates to avoid favoring one block among others
			
			// Remove the enemy
			delete pointerEnemy;
			enemiesListIterator = _enemiesList.erase(enemiesListIterator);
			
			continue;
		}
		// The enemy wants to shoot
		else if (result == 2)
		{
			pointerBullet = pointerEnemy->shoot();
			
			// Was the enemy allowed to fire ?
			if (pointerBullet != NULL) _enemiesBulletsList.push_front(pointerBullet);
		}
		
		// Enemy is still alive, check next one
		++enemiesListIterator;
	}
	
	// Check if enemies bullets have hit the player (update enemies bullets after enemies, so if they shot a new bullet is it updated too, in the same way it's done for the player. Thus, it is possible to adjust bullet spawning coordinate offsets in the same way for player and enemies)
	MovingEntityBullet *pointerEnemyBullet;
	bulletsListIterator = _enemiesBulletsList.begin();
	while (bulletsListIterator != _enemiesBulletsList.end())
	{
		pointerEnemyBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerEnemyBullet->update() != 0)
		{
			delete pointerEnemyBullet;
			bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), pointerEnemyBullet->getPositionRectangle()))
		{
			// Remove the bullet
			bulletsListIterator = _enemiesBulletsList.erase(bulletsListIterator);
			
			// Wound the player
			pointerPlayer->modifyLife(pointerEnemyBullet->getDamageAmount());
			_isPlayerHit = 1;
			LOG_DEBUG("Player hit.");
			
			// Instantly stop game updating
			if (pointerPlayer->isDead())
			{
				_isPlayerDead = 1;
				_isGamePaused = 1; // Pause game updating
				LOG_DEBUG("Player died.");
				return;
			}
			continue;
		}
		
		// Bullet is still alive, check next one
		++bulletsListIterator;
	}
	
	// Update enemy spawners at the end, to avoid new enemies being spawned in the middle of the update function
	int blockContent;
	enemySpawnersListIterator = LevelManager::enemySpawnersList.begin();
	while (enemySpawnersListIterator !=  LevelManager::enemySpawnersList.end())
	{
		pointerEnemySpawner = *enemySpawnersListIterator;
		
		result = pointerEnemySpawner->update();
		// Remove the spawner if it is destroyed
		if (result == 1)
		{
			// Remove the spawner indicator from the block
			pointerPositionRectangle = pointerEnemySpawner->getPositionRectangle();
			blockContent = LevelManager::getBlockContent(pointerPositionRectangle->x, pointerPositionRectangle->y);
			blockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
			LevelManager::setBlockContent(pointerPositionRectangle->x, pointerPositionRectangle->y, blockContent);
			
			// Display an explosion
			_animatedTexturesList.push_front(new EntityAnimatedTextureEnemySpawnerExplosion(pointerPositionRectangle->x, pointerPositionRectangle->y));
			AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_SPAWNER_EXPLOSION);
			
			// Remove the spawner
			delete pointerEnemySpawner;
			enemySpawnersListIterator = LevelManager::enemySpawnersList.erase(enemySpawnersListIterator);
			
			continue;
		}
		// Try to spawn an enemy if the spawner requested to
		else if (result == 2)
		{
			pointerPositionRectangle = pointerEnemySpawner->getPositionRectangle();
			pointerEnemy = _spawnEnemy(pointerPositionRectangle->x, pointerPositionRectangle->y);
			if (pointerEnemy != NULL) _enemiesList.push_front(pointerEnemy);
		}
		
		// Spawner is still working, check next one
		++enemySpawnersListIterator;
	}
	
	// Update animated textures at the end, because they can be spawned by previous updates
	std::list<EntityAnimatedTexture *>::iterator animatedTexturesListIterator = _animatedTexturesList.begin();
	EntityAnimatedTexture *pointerAnimatedTexture;
	while (animatedTexturesListIterator != _animatedTexturesList.end())
	{
		pointerAnimatedTexture = *animatedTexturesListIterator;
		
		if (pointerAnimatedTexture->update() != 0)
		{
			// Remove the texture
			delete pointerAnimatedTexture;
			animatedTexturesListIterator = _animatedTexturesList.erase(animatedTexturesListIterator);
		}
		// Animation is not finished, check next one
		else ++animatedTexturesListIterator;
	}
}

/** Display and keep up to date interface strings. */
static inline void _renderInterface()
{
	static int previousLifePointsAmount = -1, previousAmmunitionAmount = -1, previousSpawnersCount = -1; // Initialize everything to -1 to force the strings to be generated on game first frame
	char string[64];
	Renderer::TextColorId colorId;
	int amount;
	
	// Life points
	amount = pointerPlayer->getLifePointsAmount();
	if (amount != previousLifePointsAmount)
	{
		// Display life points in red if the player is near to death
		if (amount < 20) colorId = Renderer::TEXT_COLOR_ID_RED;
		else colorId = Renderer::TEXT_COLOR_ID_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT]); // SDL_DestroyTexture() does not complain if the provided pointer is NULL, as it is on the first frame
		
		// Render the string
		sprintf(string, "Life : %d%%", amount);
		_pointerInterfaceStrings[INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT] = Renderer::renderTextToTexture(string, colorId);
		
		previousLifePointsAmount = amount;
		LOG_DEBUG("Refreshed life points interface string.");
	}
	Renderer::renderTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT], CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X, CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y);
	
	// Ammunition count
	amount = pointerPlayer->getAmmunitionAmount();
	if (amount != previousAmmunitionAmount)
	{
		// Display ammunition in red if they are exhausted
		if (amount == 0) colorId = Renderer::TEXT_COLOR_ID_RED;
		else colorId = Renderer::TEXT_COLOR_ID_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_AMMUNITION_AMOUNT]);
		
		// Render the string
		sprintf(string, "Ammo : %d", amount);
		_pointerInterfaceStrings[INTERFACE_STRING_ID_AMMUNITION_AMOUNT] = Renderer::renderTextToTexture(string, colorId);
		
		previousAmmunitionAmount = amount;
		LOG_DEBUG("Refreshed ammunition interface string.");
	}
	Renderer::renderTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_AMMUNITION_AMOUNT], CONFIGURATION_DISPLAY_HUD_AMMUNITION_X, CONFIGURATION_DISPLAY_HUD_AMMUNITION_Y);
	
	// Remaining enemies count
	amount = (int) _enemiesList.size();
	if (amount != previousSpawnersCount)
	{
		// Display enemies in green if they are all dead
		if (amount == 0) colorId = Renderer::TEXT_COLOR_ID_GREEN;
		else colorId = Renderer::TEXT_COLOR_ID_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_ENEMIES_COUNT]);
		
		// Render the string
		sprintf(string, "Enemies : %d", amount);
		_pointerInterfaceStrings[INTERFACE_STRING_ID_ENEMIES_COUNT] = Renderer::renderTextToTexture(string, colorId);
		
		previousSpawnersCount = amount;
		LOG_DEBUG("Refreshed enemies interface string.");
	}
	Renderer::renderTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_ENEMIES_COUNT], CONFIGURATION_DISPLAY_HUD_ENEMIES_X, CONFIGURATION_DISPLAY_HUD_ENEMIES_Y);
	
	// Display a centered message if needed
	if (_isPlayerDead) Renderer::renderCenteredTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_GAME_LOST]);
	else if (_isGameFinished) Renderer::renderCenteredTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_GAME_WON]);
	else if (_isGamePaused) Renderer::renderCenteredTexture(_pointerInterfaceStrings[INTERFACE_STRING_ID_GAME_PAUSED]);
}

/** Display everything to the screen. */
static inline void _renderGame()
{
	int sceneX, sceneY;
	
	// Compute rendering top left coordinates
	sceneX = pointerPlayer->getX() - _cameraOffsetX;
	sceneY = pointerPlayer->getY() - _cameraOffsetY;
	
	// Start rendering
	Renderer::beginRendering(sceneX, sceneY);
	
	// Render the level walls
	LevelManager::renderScene(sceneX, sceneY);
	
	// Display enemies
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator) (*enemiesListIterator)->render();
	
	// Display enemy spawners
	std::list<EntityEnemySpawner *>::iterator enemySpawnersListIterator;
	for (enemySpawnersListIterator = LevelManager::enemySpawnersList.begin(); enemySpawnersListIterator != LevelManager::enemySpawnersList.end(); ++enemySpawnersListIterator) (*enemySpawnersListIterator)->render();
	
	// Display bullets after enemies, so when multiple enemies fire on themselves bullets are visible on top of enemies
	std::list<MovingEntityBullet *>::iterator bulletsListIterator;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	
	// Display the player now, so it is always rendered on top on everything else and can always be visible
	pointerPlayer->render();
	
	// Display special effects at the end, so they can recover everything
	std::list<EntityAnimatedTexture *>::iterator animatedTexturesListIterator;
	for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) (*animatedTexturesListIterator)->render();
	
	// Display the red overlay
	if (_isPlayerHit)
	{
		_pointerPlayerHitOverlayTexture->render();
		_isPlayerHit = 0;
	}
	
	// Display HUD
	_renderInterface();
	
	// Display the rendered picture
	SDL_RenderPresent(Renderer::pointerMainRenderer);
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	SDL_Event event;
	unsigned int frameStartingTime, frameElapsedTime;
	int isKeyPressed[KEYBOARD_KEY_IDS_COUNT] = {0}, isLastDirectionVertical = 1, isFullScreenEnabled = 0, levelToLoadNumber;
	#if CONFIGURATION_DISPLAY_IS_FRAME_RATE_DISPLAYING_ENABLED
		unsigned int frameRateStartingTime = 0;
		int framesCount = 0;
	#endif
	
	// Check parameters
	if (argc > 1)
	{
		// Is full screen mode requested ?
		if (strcmp("-f", argv[1]) == 0) isFullScreenEnabled = 1;
		else
		{
			printf("Unknown parameter(s).\n"
				"Usage : %s [-f]\n"
				" -f : enable full screen\n", argv[0]);
			return -1;
		}
	}
	
	// Engine initialization
	if (Renderer::initialize(isFullScreenEnabled) != 0) return -1;
	if (TextureManager::initialize() != 0) return -1;
	if (LevelManager::initialize() != 0) return -1;
	if (AudioManager::initialize() != 0) return -1;
	
	// Create the player now that everything is working
	pointerPlayer = new FightingEntityPlayer(0, 0); // It will be placed at the right location by the level loading function
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
	atexit(_exitFreeResources);
	
	// Initialize pseudo-random numbers generator
	srand(time(NULL));
	
	// Cache some values
	_enemySpawnOffsetX = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_SMALL_ENEMY_FACING_UP)->getWidth() / 2; // All enemies have same dimensions
	_enemySpawnOffsetY = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_SMALL_ENEMY_FACING_UP)->getHeight() / 2;
	// Offset to subtract to the player position to have the scene camera coordinates
	_cameraOffsetX = (Renderer::displayWidth / 2) - (TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_UP)->getWidth() / 2);
	_cameraOffsetY = (Renderer::displayHeight / 2) - (TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_UP)->getHeight() / 2);
	// Player damage overlay
	_pointerPlayerHitOverlayTexture = (TextureDisplayOverlay *) TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_HIT_OVERLAY);
	// Static interface strings
	_pointerInterfaceStrings[INTERFACE_STRING_ID_GAME_PAUSED] = Renderer::renderTextToTexture("PAUSE", Renderer::TEXT_COLOR_ID_BLUE);
	_pointerInterfaceStrings[INTERFACE_STRING_ID_GAME_LOST] =  Renderer::renderTextToTexture("You are dead ! Hit R to retry.", Renderer::TEXT_COLOR_ID_BLUE);
	_pointerInterfaceStrings[INTERFACE_STRING_ID_GAME_WON] = Renderer::renderTextToTexture("All levels completed. You are legend.", Renderer::TEXT_COLOR_ID_BLUE);
	
	LOG_INFORMATION("Game engine successfully initialized.");
	
	// Load first level
	_loadNextLevel();
	AudioManager::playMusic();
	
	while (1)
	{
		// Store the time when the loop started
		frameStartingTime = SDL_GetTicks();
		
		#if CONFIGURATION_DISPLAY_IS_FRAME_RATE_DISPLAYING_ENABLED
			if (frameRateStartingTime == 0) frameRateStartingTime = frameStartingTime;
		#endif
		
		// Handle all relevant events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto Exit;
				
				// Remember which key are pressed
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 1;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 0;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_DOWN:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 0;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 1;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_LEFT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 1;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 0;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_RIGHT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 0;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 1;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_SPACE:
							isKeyPressed[KEYBOARD_KEY_ID_SPACE] = 1;
							break;
							
						case SDL_SCANCODE_ESCAPE:
							// Pause or continue the game (only if the player is alive, or it would allow the game to continue even if the player is dead)
							if (_isPlayerDead || _isGameFinished) break;
							
							_isGamePaused = !_isGamePaused;
							if (_isGamePaused) LOG_INFORMATION("Game paused.");
							else LOG_INFORMATION("Game continuing.");
							
							AudioManager::pauseMusic(_isGamePaused);
							break;
						
						// Restart current level
						case SDL_SCANCODE_R:
							// Can't restart a level if the game is paused or if all levels are completed
							if ((_isGamePaused && (!_isPlayerDead)) || _isGameFinished) break;
							
							// Stop currently playing sounds
							AudioManager::stopAllSounds();
							
							// Free all entities
							_clearAllLists();
							
							// Choose level number to load (_loadNextLevel() automatically increments _currentLevelNumber)
							if (_currentLevelNumber == 0) levelToLoadNumber = 0;
							else levelToLoadNumber = _currentLevelNumber - 1;
							
							// Try to load the level
							if (LevelManager::loadLevel(levelToLoadNumber) != 0)
							{
								LOG_ERROR("Failed to reload level %d.", levelToLoadNumber);
								exit(-1);
							}
							
							// Restore player life and force ammunition count (so a player can accumulate a huge amount of ammunition by restarting the level several times)
							pointerPlayer->modifyLife(100);
							pointerPlayer->setAmmunitionAmount(100);
							
							// Allow the game to restart if the player is dead
							if (_isPlayerDead)
							{
								_isPlayerDead = 0;
								_isGamePaused = 0;
							}
							break;
							
						default:
							break;
					}
					break;
				}
				
				// Some key has been released
				case SDL_KEYUP:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 0;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_DOWN:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 0;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_LEFT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 0;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_RIGHT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 0;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_SPACE:
							isKeyPressed[KEYBOARD_KEY_ID_SPACE] = 0;
							break;
							
						default:
							break;
					}
					break;
				}
			}
		}
		
		// Do not update the game anymore if the player died
		if (!_isGamePaused)
		{
			// React to player key press without depending of keyboard key repetition rate
			// Handle both vertical and horizontal direction movement
			if ((isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] || isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN]) && (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] || isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT]))
			{
				// Keep trace of the last direction the player took to favor it, when this key will be released the previous direction will be favored
				if (isLastDirectionVertical)
				{
					if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP]) pointerPlayer->moveToUp();
					else pointerPlayer->moveToDown();
				}
				else
				{
					if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT]) pointerPlayer->moveToLeft();
					else pointerPlayer->moveToRight();
				}
			}
			// Handle a single key press
			else
			{
				if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP]) pointerPlayer->moveToUp();
				else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN]) pointerPlayer->moveToDown();
				else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT]) pointerPlayer->moveToLeft();
				else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT]) pointerPlayer->moveToRight();
			}
			
			// Fire a bullet
			if (isKeyPressed[KEYBOARD_KEY_ID_SPACE])
			{
				MovingEntityBullet *pointerBullet = pointerPlayer->shoot();
				
				// Is the player allowed to shoot ?
				if (pointerBullet != NULL) _playerBulletsList.push_front(pointerBullet);
			}
			
			_updateGameLogic();
		}
		
		_renderGame();
		
		// Wait enough time to achieve a 60Hz refresh rate
		frameElapsedTime = SDL_GetTicks() - frameStartingTime;
		if (frameElapsedTime < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - frameElapsedTime);
		
		#if CONFIGURATION_DISPLAY_IS_FRAME_RATE_DISPLAYING_ENABLED
			framesCount++;
			// Display rendered frames count each second
			if (SDL_GetTicks() - frameRateStartingTime >= 1000)
			{
				LOG_INFORMATION("Frame rate : %d fps", framesCount);
				framesCount = 0;
				frameRateStartingTime = 0;
			}
		#endif
	}
	
Exit:
	return 0;
}
