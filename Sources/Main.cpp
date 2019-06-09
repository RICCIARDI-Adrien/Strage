/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <BigEnemyFightingEntity.hpp>
#include <BulletMovingEntity.hpp>
#include <Configuration.hpp>
#include <ControlManager.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <EnemyFightingEntity.hpp>
#include <EnemySpawnerStaticEntity.hpp>
#include <FileManager.hpp>
#include <HeadUpDisplay.hpp>
#include <list>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <MediumEnemyFightingEntity.hpp>
#include <Menu.hpp>
#include <PlayerFightingEntity.hpp>
#include <Renderer.hpp>
#include <SavegameManager.hpp>
#include <SDL2/SDL.h>
#include <SmallEnemyFightingEntity.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All bullets shot by the player. */
static std::list<BulletMovingEntity *> _playerBulletsList;
/** All bullets shot by the enemies. */
static std::list<BulletMovingEntity *> _enemiesBulletsList;

/** All enemies. */
static std::list<EnemyFightingEntity *> _enemiesList;

/** How many pixels to subtract to the player X coordinate to obtain the scene camera X coordinate. */
static int _cameraOffsetX;
/** How many pixels to subtract to the player Y coordinate to obtain the scene camera Y coordinate. */
static int _cameraOffsetY;

/** When set to 1, stop game updating and display a text saying that the player is dead. */
static int _isPlayerDead = 0;
/** Set to 1 when the player has been hit. */
static int _isPlayerHit = 0;

/** Draw the screen in red if the player has been hit. */
static Texture *_pointerPlayerHitOverlayTexture;

/** The current level number. */
static int _currentLevelNumber = 0;

/** When set to 1, stop game updating but continue displaying the scene. */
static int _isGamePaused = 0;
/** Set to 1 when the player has finished all levels. */
static int _isGameFinished = 0;

/** The string to display when the player is dead. */
static SDL_Texture *_pointerGameLostInterfaceStringTexture;
/** The string to display when the player won the game. */
static SDL_Texture *_pointerGameWonInterfaceStringTexture;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
// The variable must be outside of any namespace
PlayerFightingEntity *pointerPlayer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Free all lists content. */
static void _clearAllLists()
{
	std::list<EnemySpawnerStaticEntity *>::iterator enemySpawnersListIterator;
	for (enemySpawnersListIterator = LevelManager::enemySpawnersList.begin(); enemySpawnersListIterator != LevelManager::enemySpawnersList.end(); ++enemySpawnersListIterator) delete *enemySpawnersListIterator;
	LevelManager::enemySpawnersList.clear();
	
	std::list<BulletMovingEntity *>::iterator bulletsListIterator;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator) delete *bulletsListIterator;
	_playerBulletsList.clear();
	
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator) delete *bulletsListIterator;
	_enemiesBulletsList.clear();

	std::list<EnemyFightingEntity *>::iterator enemiesListIterator;
	for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator) delete *enemiesListIterator;
	_enemiesList.clear();
	
	EffectManager::clearAllEffects();
}

/** Automatically free allocated resources on program shutdown. */
static void _exitFreeResources()
{
	// Free all cached interface strings
	SDL_DestroyTexture(_pointerGameLostInterfaceStringTexture);
	SDL_DestroyTexture(_pointerGameWonInterfaceStringTexture);
	
	// Delete all entities
	_clearAllLists();
	
	delete pointerPlayer;
	
	ControlManager::uninitialize();
	AudioManager::uninitialize();
	LevelManager::uninitialize();
	TextureManager::uninitialize();
	Renderer::uninitialize();
	
	SDL_Quit();
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
static inline EnemyFightingEntity *_spawnEnemy(int enemySpawnerX, int enemySpawnerY)
{
	int x, y, spawningPercentage;
	EnemyFightingEntity *pointerEnemy;
	SDL_Rect *pointerEnemyPositionRectangle;
	
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
	// Select which enemy to spawn
	spawningPercentage = rand() % 100;
	// Start with the smaller percentage and continue so on
	if (spawningPercentage <= CONFIGURATION_GAMEPLAY_BIG_ENEMY_SPAWN_PROBABILITY_PERCENTAGE) pointerEnemy = new BigEnemyFightingEntity(x, y); // Spawn the enemy on the top-left part of the block, as the enemy size can't be known until it is instantiated
	else if (spawningPercentage <= CONFIGURATION_GAMEPLAY_MEDIUM_ENEMY_SPAWN_PROBABILITY_PERCENTAGE) pointerEnemy = new MediumEnemyFightingEntity(x, y);
	else pointerEnemy = new SmallEnemyFightingEntity(x, y);
	
	// Adjust coordinates to spawn the enemy at the block center
	pointerEnemyPositionRectangle = pointerEnemy->getPositionRectangle();
	x += (CONFIGURATION_LEVEL_BLOCK_SIZE - pointerEnemyPositionRectangle->w) / 2;
	y += (CONFIGURATION_LEVEL_BLOCK_SIZE - pointerEnemyPositionRectangle->h) / 2;
	pointerEnemy->setX(x);
	pointerEnemy->setY(y);
	
	LOG_DEBUG("Spawned an enemy on map coordinates (%d, %d).", x, y);
	return pointerEnemy;
}

/** Load next level. */
static inline void _loadNextLevel()
{
	// Are all levels completed ?
	if (_currentLevelNumber == CONFIGURATION_LEVELS_COUNT)
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
	
	// Save player progress
	SavegameManager::setSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_STARTING_LEVEL, _currentLevelNumber);
	SavegameManager::setSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_PLAYER_MAXIMUM_LIFE_POINTS, pointerPlayer->getMaximumLifePointsAmount());
	SavegameManager::setSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_PLAYER_AMMUNITION, pointerPlayer->getAmmunitionAmount());
	SavegameManager::storeSavegame();
	LOG_DEBUG("Saved game.");
	
	// Prepare to load next level
	_currentLevelNumber++;
}

/** Update all game actors. */
static inline void _updateGameLogic()
{
	static unsigned int lastEnemySpawningTime = -CONFIGURATION_GAMEPLAY_TIME_BETWEEN_ENEMY_SPAWN; // Initialize to a negative value (gcc allows it) to spawn an enemy immediately when the game starts
	
	// Check if pickable objects can be taken by the player or if the level end has been reached
	if (pointerPlayer->update() == 2)
	{
		// Next level can be loaded only if all enemies are dead and all spawners are destroyed
		if (_enemiesList.empty() && LevelManager::enemySpawnersList.empty())
		{
			// Restore player maximum life
			pointerPlayer->modifyLife(pointerPlayer->getMaximumLifePointsAmount());
			
			_loadNextLevel();
			return;
		}
	}
	
	// Check if player bullets have hit a wall or an enemy
	std::list<BulletMovingEntity *>::iterator bulletsListIterator = _playerBulletsList.begin();
	std::list<EnemyFightingEntity *>::iterator enemiesListIterator;
	std::list<EnemySpawnerStaticEntity *>::iterator enemySpawnersListIterator;
	BulletMovingEntity *pointerPlayerBullet;
	EnemyFightingEntity *pointerEnemy;
	EnemySpawnerStaticEntity *pointerEnemySpawner;
	while (bulletsListIterator != _playerBulletsList.end())
	{
		pointerPlayerBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerPlayerBullet->update() != 0)
		{
			EffectManager::addEffect(pointerPlayerBullet->getX() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, pointerPlayerBullet->getY() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, EffectManager::EFFECT_ID_BULLET_EXPLOSION_NO_SOUND);
			
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
				LOG_DEBUG("Enemy hit.");
				
				EffectManager::addEffect(pointerPlayerBullet->getX() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, pointerPlayerBullet->getY() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, EffectManager::EFFECT_ID_BULLET_EXPLOSION_ENEMY_HIT);
				
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
				LOG_DEBUG("Enemy spawner hit.");
				
				EffectManager::addEffect(pointerPlayerBullet->getX() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, pointerPlayerBullet->getY() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, EffectManager::EFFECT_ID_BULLET_EXPLOSION_ENEMY_SPAWNER_HIT);
				
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
	BulletMovingEntity *pointerBullet;
	int result;
	int previousEnemiesAmount = _enemiesList.size();
	SDL_Rect *pointerPositionRectangle;
	enemiesListIterator = _enemiesList.begin();
	while (enemiesListIterator != _enemiesList.end())
	{
		pointerEnemy = *enemiesListIterator;
		
		result = pointerEnemy->update();
		// Remove the enemy if it is dead
		if (result == 1)
		{
			// Spawn an item on the current block if player is lucky
			pointerPositionRectangle = pointerEnemy->getPositionRectangle();
			_spawnItem(pointerPositionRectangle->x + (pointerPositionRectangle->w / 2), pointerPositionRectangle->y + (pointerPositionRectangle->h / 2)); // Use enemy center coordinates to avoid favoring one block among others
			
			// Remove the enemy
			delete pointerEnemy;
			enemiesListIterator = _enemiesList.erase(enemiesListIterator);
			
			continue;
		}
		// The enemy wants to shoot
		else if (result == 2)
		{
			// Is the enemy allowed to fire ?
			pointerBullet = pointerEnemy->shoot();
			if (pointerBullet != NULL) _enemiesBulletsList.push_front(pointerBullet);
		}
		
		// Enemy is still alive, check next one
		++enemiesListIterator;
	}
	
	// Check if enemies bullets have hit the player (update enemies bullets after enemies, so if they shot a new bullet is it updated too, in the same way it's done for the player. Thus, it is possible to adjust bullet spawning coordinate offsets in the same way for player and enemies)
	BulletMovingEntity *pointerEnemyBullet;
	bulletsListIterator = _enemiesBulletsList.begin();
	pointerPositionRectangle = pointerPlayer->getPositionRectangle();
	while (bulletsListIterator != _enemiesBulletsList.end())
	{
		pointerEnemyBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerEnemyBullet->update() != 0)
		{
			EffectManager::addEffect(pointerEnemyBullet->getX() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, pointerEnemyBullet->getY() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, EffectManager::EFFECT_ID_BULLET_EXPLOSION_NO_SOUND);
			
			delete pointerEnemyBullet;
			bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		if (SDL_HasIntersection(pointerPositionRectangle, pointerEnemyBullet->getPositionRectangle()))
		{
			// Wound the player
			pointerPlayer->modifyLife(pointerEnemyBullet->getDamageAmount());
			_isPlayerHit = 1;
			LOG_DEBUG("Player hit.");
			
			EffectManager::addEffect(pointerEnemyBullet->getX() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, pointerEnemyBullet->getY() + CONFIGURATION_BULLET_EXPLOSION_POSITION_OFFSET, EffectManager::EFFECT_ID_BULLET_EXPLOSION_NO_SOUND);
			
			// Remove the bullet
			delete pointerEnemyBullet;
			bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
			
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
	int blockContent, isEnemySpawned = 0;
	enemySpawnersListIterator = LevelManager::enemySpawnersList.begin();
	// Should enemies be spawned ?
	if (SDL_GetTicks() - lastEnemySpawningTime >= CONFIGURATION_GAMEPLAY_TIME_BETWEEN_ENEMY_SPAWN)
	{
		lastEnemySpawningTime = SDL_GetTicks();
		isEnemySpawned = 1;
	}
	while (enemySpawnersListIterator !=  LevelManager::enemySpawnersList.end())
	{
		pointerEnemySpawner = *enemySpawnersListIterator;
		
		// Remove the spawner if it is destroyed
		if (pointerEnemySpawner->update() == 1)
		{
			// Remove the spawner indicator from the block
			pointerPositionRectangle = pointerEnemySpawner->getPositionRectangle();
			blockContent = LevelManager::getBlockContent(pointerPositionRectangle->x, pointerPositionRectangle->y);
			blockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
			LevelManager::setBlockContent(pointerPositionRectangle->x, pointerPositionRectangle->y, blockContent);
			
			// Display an explosion
			EffectManager::addEffect(pointerPositionRectangle->x, pointerPositionRectangle->y, EffectManager::EFFECT_ID_ENEMY_SPAWNER_EXPLOSION);
			
			// Remove the spawner
			delete pointerEnemySpawner;
			enemySpawnersListIterator = LevelManager::enemySpawnersList.erase(enemySpawnersListIterator);
			
			continue;
		}
		// Try to spawn an enemy if the spawner is still alive and enough time has elapsed since last spawn
		else if (isEnemySpawned)
		{
			pointerPositionRectangle = pointerEnemySpawner->getPositionRectangle();
			pointerEnemy = _spawnEnemy(pointerPositionRectangle->x, pointerPositionRectangle->y);
			if (pointerEnemy != NULL) _enemiesList.push_front(pointerEnemy);
		}
		
		// Spawner is still working, check next one
		++enemySpawnersListIterator;
	}
	
	// Update effects at the end because they can be spawned by previous updates
	EffectManager::update();
	
	// Update HUD enemies count if changed
	int currentEnemiesAmount = _enemiesList.size();
	if (currentEnemiesAmount != previousEnemiesAmount) HeadUpDisplay::setEnemiesAmount(currentEnemiesAmount);
}

/** Display and keep up to date interface strings. */
static inline void _renderInterface()
{
	HeadUpDisplay::render();
	
	// Display a centered message if needed
	if (_isPlayerDead) Renderer::renderCenteredTexture(_pointerGameLostInterfaceStringTexture);
	else if (_isGameFinished) Renderer::renderCenteredTexture(_pointerGameWonInterfaceStringTexture);
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
	
	// Render the level walls and static objects (ammunition, medipacks, ...)
	LevelManager::renderScene(sceneX, sceneY);
	
	// Display enemy spawners
	std::list<EnemySpawnerStaticEntity *>::iterator enemySpawnersListIterator;
	for (enemySpawnersListIterator = LevelManager::enemySpawnersList.begin(); enemySpawnersListIterator != LevelManager::enemySpawnersList.end(); ++enemySpawnersListIterator) (*enemySpawnersListIterator)->render();
	
	// Display enemies
	std::list<EnemyFightingEntity *>::iterator enemiesListIterator;
	for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator) (*enemiesListIterator)->render();
	
	// Display bullets after enemies, so when multiple enemies fire on themselves bullets are visible on top of enemies
	std::list<BulletMovingEntity *>::iterator bulletsListIterator;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	
	// Display the player now, so it is always rendered on top on everything else and can always be visible
	pointerPlayer->render();
	
	// Display special effects at the end, so they can recover everything
	EffectManager::render();
	
	// Display the red overlay
	if (_isPlayerHit)
	{
		SDL_RenderCopy(Renderer::pointerRenderer, _pointerPlayerHitOverlayTexture->getSDLTexture(), NULL, NULL);
		_isPlayerHit = 0;
	}
	
	// Display HUD
	_renderInterface();
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	SDL_Event event;
	unsigned int frameStartingTime, frameElapsedTime, frameRateStartingTime = 0;
	bool isFullScreenEnabled = true, isFramesPerSecondDisplayingEnabled = false;
	int levelToLoadNumber, i, framesCount = 0;
	BulletMovingEntity *pointerBullet;
	SDL_Texture *pointerFramesPerSecondSdlTexture = NULL;
	char stringFramesPerSecond[16];
	const char *pointerStringsMainMenuWithSavegameItems[] =
	{
		"Continue game",
		"New game",
		"Controls",
		"Quit"
	};
	const char *pointerStringsMainMenuWithoutSavegameItems[] =
	{
		"New game",
		"Controls",
		"Quit"
	};
	const char *pointerStringsPauseMenuItems[] =
	{
		"Continue",
		"Restart level",
		"Quit"
	};
	const char *pointerStringsVictoryMenuItems[] =
	{
		"Relish your victory",
		"Quit"
	};
	
	// Check parameters
	if (argc > 1)
	{
		// Process all parameters
		for (i = 1; i < argc; i++)
		{
			// Is FPS displaying requested ?
			if (strcmp("-fps", argv[i]) == 0) isFramesPerSecondDisplayingEnabled = true;
			// Is full screen mode requested ?
			else if (strcmp("-windowed", argv[i]) == 0) isFullScreenEnabled = false;
			// Display help if a provided parameter is unknown
			else
			{
				printf("Usage : %s [option].\n"
					"Available options :\n"
					"  -fps      : display frames per second count.\n"
					"  -help     : display this help and exit.\n"
					"  -windowed : play game in windowed screen mode.\n"
					"All other parameters are ignored.\n", argv[0]);
				return 0;
			}
		}
	}
	
	// Initialize the needed SDL subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0)
	{
		LOG_ERROR("SDL_Init() failed (%s).", SDL_GetError());
		return -1;
	}
	
	// Engine initialization
	if (FileManager::initialize() != 0) return -1; // Must be initialized before the managers that use it
	if (Renderer::initialize(isFullScreenEnabled) != 0) return -1;
	if (TextureManager::initialize() != 0) return -1;
	if (LevelManager::initialize() != 0) return -1;
	if (AudioManager::initialize() != 0) return -1;
	if (ControlManager::initialize() != 0) return -1;
	if (HeadUpDisplay::initialize() != 0) return -1;
	
	// Create the player now that everything is working
	pointerPlayer = new PlayerFightingEntity(0, 0); // It will be placed at the right location by the level loading function
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
	atexit(_exitFreeResources);
	
	// Initialize pseudo-random numbers generator
	srand(time(NULL));
	
	// Cache some values
	// Offset to subtract to the player position to have the scene camera coordinates
	_cameraOffsetX = (Renderer::displayWidth / 2) - (TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_UP)->getWidth() / 2);
	_cameraOffsetY = (Renderer::displayHeight / 2) - (TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_UP)->getHeight() / 2);
	// Player damage overlay
	_pointerPlayerHitOverlayTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_HIT_OVERLAY);
	// Static interface strings
	_pointerGameLostInterfaceStringTexture =  Renderer::renderTextToTexture("You are dead !", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	_pointerGameWonInterfaceStringTexture = Renderer::renderTextToTexture("All levels completed. You are legend.", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	
	LOG_INFORMATION("Game engine successfully initialized.");
	
	// Display the main menu and get user choice
	bool isMainMenuDisplayed = true;
	bool isSavegamePresent = SavegameManager::isSavegamePresent(); // Cache value to avoid checking for the file every time the menu is displayed
	do
	{
		if (isSavegamePresent)
		{
			switch (Menu::display("Main menu", pointerStringsMainMenuWithSavegameItems, 4))
			{
				// Continue last saved game
				case 0:
					// Is a savegame available ?
					if (SavegameManager::loadSavegame() == 0)
					{
						// Set which level to load
						_currentLevelNumber = SavegameManager::getSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_STARTING_LEVEL);
						
						// Set player life points
						i = SavegameManager::getSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_PLAYER_MAXIMUM_LIFE_POINTS); // Recycle 'i' variable
						pointerPlayer->setLifePointsAmount(i);
						pointerPlayer->setMaximumLifePointsAmount(i);
						
						// Set player ammunition
						pointerPlayer->setAmmunitionAmount(SavegameManager::getSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_PLAYER_AMMUNITION));
					}
					else LOG_ERROR("No valid savegame found, starting a new game.");
					isMainMenuDisplayed = false;
					break;
					
				// Start a new game (nothing to do because all needed variables are already initialized)
				case 1:
					isMainMenuDisplayed = false;
					break;
					
				// Display controls menu
				case 2:
					if (Menu::displayControlsMenu() != 0) goto Exit;
					break;
					
				default:
					goto Exit;
			}
		}
		else
		{
			switch (Menu::display("Main menu", pointerStringsMainMenuWithoutSavegameItems, 3))
			{
				// Start a new game (nothing to do because all needed variables are already initialized)
				case 0:
					isMainMenuDisplayed = false;
					break;
					
				// Display controls menu
				case 1:
					if (Menu::displayControlsMenu() != 0) goto Exit;
					break;
					
				default:
					goto Exit;
			}
		}
	} while (isMainMenuDisplayed);
	
	// Load first level
	_loadNextLevel();
	AudioManager::playMusic();
	
	// Create a fake texture to be used by the FPS displaying code, this avoids multiple checks to detect whether the first texture is null
	if (isFramesPerSecondDisplayingEnabled) pointerFramesPerSecondSdlTexture = Renderer::renderTextToTexture("--", Renderer::TEXT_COLOR_ID_RED, Renderer::FONT_SIZE_ID_BIG); // Provided text can have a zero character size or it would trigger a SDL error
	
	while (1)
	{
		// Store the time when the loop started
		frameStartingTime = SDL_GetTicks();
		if (isFramesPerSecondDisplayingEnabled)
		{
			if (frameRateStartingTime == 0) frameRateStartingTime = frameStartingTime;
		}
		
		// Handle all relevant events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto Exit;
					
				case SDL_CONTROLLERBUTTONUP:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERAXISMOTION:
					ControlManager::handleGameControllerEvent(&event);
					break;
					
				case SDL_KEYUP:
				case SDL_KEYDOWN:
					ControlManager::handleKeyboardEvent(&event);
					break;
			}
		}
		
		// Pause or continue the game
		if (ControlManager::isKeyPressed(ControlManager::KEY_ID_PAUSE_GAME))
		{
			// Stop playing music while the game is paused (in case the game must be quickly hidden to an incoming person)
			LOG_DEBUG("Game paused.");
			AudioManager::pauseMusic(1);
			
			// Player won, display a specific menu
			if (_isGameFinished)
			{
				if (Menu::display("Victory !", pointerStringsVictoryMenuItems, 2) == 1) goto Exit;
			}
			// Normal pause menu
			else
			{
				switch (Menu::display("Pause", pointerStringsPauseMenuItems, 3))
				{
					// Player has selected "Continue"
					case 0:
						break;
					
					// Player has selected "Restart level"
					case 1:
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
						
						// Restore player life and ammunition count as they were at the level start
						i = SavegameManager::getSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_PLAYER_MAXIMUM_LIFE_POINTS); // Recycle 'i' variable
						pointerPlayer->setLifePointsAmount(i);
						pointerPlayer->setMaximumLifePointsAmount(i);
						pointerPlayer->setAmmunitionAmount(SavegameManager::getSavegameItem(SavegameManager::SAVEGAME_ITEM_ID_PLAYER_AMMUNITION));
						
						// Allow the game to restart if the player is dead
						if (_isPlayerDead)
						{
							_isPlayerDead = 0;
							_isGamePaused = 0;
						}
						break;
					
					default:
						goto Exit;
				}
			}
			
			LOG_DEBUG("Game continuing.");
			AudioManager::pauseMusic(0);
		}
		
		// Do not update the game anymore if the player died
		if (!_isGamePaused)
		{
			// React to player key press without depending of keyboard key repetition rate
			// Handle both vertical and horizontal direction movement
			if ((ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_UP) || ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_DOWN)) && (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_LEFT) || ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_RIGHT)))
			{
				// Keep trace of the last direction the player took to favor it, when this key will be released the previous direction will be favored
				if (ControlManager::isLastPressedDirectionKeyOnVerticalAxis())
				{
					if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_UP)) pointerPlayer->moveToUp();
					else pointerPlayer->moveToDown();
				}
				else
				{
					if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_LEFT)) pointerPlayer->moveToLeft();
					else pointerPlayer->moveToRight();
				}
			}
			// Handle a single key press
			else
			{
				if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_UP)) pointerPlayer->moveToUp();
				else if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_DOWN)) pointerPlayer->moveToDown();
				else if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_LEFT)) pointerPlayer->moveToLeft();
				else if (ControlManager::isKeyPressed(ControlManager::KEY_ID_GO_RIGHT)) pointerPlayer->moveToRight();
			}
			
			// Fire a bullet
			if (ControlManager::isKeyPressed(ControlManager::KEY_ID_PRIMARY_SHOOT))
			{
				// Is the player allowed to shoot ?
				pointerBullet = pointerPlayer->shoot();
				if (pointerBullet != NULL) _playerBulletsList.push_front(pointerBullet);
			}
			// Fire a mortar shell (bullet and mortar shell can be shot in the same time)
			if (ControlManager::isKeyPressed(ControlManager::KEY_ID_SECONDARY_SHOOT))
			{
				// Is the player allowed to shoot ?
				pointerBullet = pointerPlayer->shootSecondaryFire();
				if (pointerBullet != NULL) _playerBulletsList.push_front(pointerBullet);
			}
			
			_updateGameLogic();
		}
		
		_renderGame();
		
		// Append the FPS string before displaying the rendered frame
		if (isFramesPerSecondDisplayingEnabled)
		{
			// Update rendered frames count each second
			if (SDL_GetTicks() - frameRateStartingTime >= 1000)
			{
				// Render the FPS count to a string
				SDL_DestroyTexture(pointerFramesPerSecondSdlTexture); // Free previously created texture
				sprintf(stringFramesPerSecond, "%d", framesCount);
				pointerFramesPerSecondSdlTexture = Renderer::renderTextToTexture(stringFramesPerSecond, Renderer::TEXT_COLOR_ID_RED, Renderer::FONT_SIZE_ID_BIG);
				
				// Restart computation
				framesCount = 0;
				frameRateStartingTime = 0;
			}
			else framesCount++;
			
			// Display the FPS count to the screen top right side at each frame
			Renderer::renderTexture(pointerFramesPerSecondSdlTexture, Renderer::displayWidth - 100, 20);
		}
		Renderer::endRendering();
		
		// Wait enough time to achieve a 60Hz refresh rate
		frameElapsedTime = SDL_GetTicks() - frameStartingTime;
		if (frameElapsedTime < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - frameElapsedTime);
	}
	
Exit:
	return 0;
}
