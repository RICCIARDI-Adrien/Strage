/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <Configuration.hpp>
#include <ControlManager.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <FightingEntityEnemy.hpp>
#include <FightingEntityPlayer.hpp>
#include <FightingEntityEnemyBig.hpp>
#include <FightingEntityEnemyMedium.hpp>
#include <FightingEntityEnemySmall.hpp>
#include <FileManager.hpp>
#include <list>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <Menu.hpp>
#include <MovingEntityBullet.hpp>
#include <Renderer.hpp>
#include <SavegameManager.hpp>
#include <SDL2/SDL.h>
#include <StaticEntityEnemySpawner.hpp>
#include <TextureManager.hpp>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All strings the interface can display. */
typedef enum
{
	INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT,
	INTERFACE_STRING_ID_AMMUNITION_AMOUNT,
	INTERFACE_STRING_ID_ENEMIES_COUNT,
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

/** Cache all interface strings (rendered to textures) because they are really slow to render. */
static SDL_Texture *_pointerInterfaceStringTextures[INTERFACE_STRING_IDS_COUNT];
/** Point to the interface background texture with the right type for the rendering function. */
static SDL_Texture *_pointerInterfaceBackgroundTexture;

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
	std::list<StaticEntityEnemySpawner *>::iterator enemySpawnersListIterator;
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
	
	EffectManager::clearAllEffects();
}

/** Automatically free allocated resources on program shutdown. */
static void _exitFreeResources()
{
	int i;
	
	// Free all cached interface strings
	for (i = 0; i < INTERFACE_STRING_IDS_COUNT; i++) SDL_DestroyTexture(_pointerInterfaceStringTextures[i]);
	
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
static inline FightingEntityEnemy *_spawnEnemy(int enemySpawnerX, int enemySpawnerY)
{
	int x, y, spawningPercentage;
	FightingEntityEnemy *pointerEnemy;
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
	if (spawningPercentage <= CONFIGURATION_GAMEPLAY_BIG_ENEMY_SPAWN_PROBABILITY_PERCENTAGE) pointerEnemy = new FightingEntityEnemyBig(x, y); // Spawn the enemy on the top-left part of the block, as the enemy size can't be known until it is instantiated
	else if (spawningPercentage <= CONFIGURATION_GAMEPLAY_MEDIUM_ENEMY_SPAWN_PROBABILITY_PERCENTAGE) pointerEnemy = new FightingEntityEnemyMedium(x, y);
	else pointerEnemy = new FightingEntityEnemySmall(x, y);
	
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
	std::list<MovingEntityBullet *>::iterator bulletsListIterator = _playerBulletsList.begin();
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	std::list<StaticEntityEnemySpawner *>::iterator enemySpawnersListIterator;
	MovingEntityBullet *pointerPlayerBullet;
	FightingEntityEnemy *pointerEnemy;
	StaticEntityEnemySpawner *pointerEnemySpawner;
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
	MovingEntityBullet *pointerEnemyBullet;
	bulletsListIterator = _enemiesBulletsList.begin();
	pointerPositionRectangle = pointerPlayer->getPositionRectangle();
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
		
		if (SDL_HasIntersection(pointerPositionRectangle, pointerEnemyBullet->getPositionRectangle()))
		{
			// Wound the player
			pointerPlayer->modifyLife(pointerEnemyBullet->getDamageAmount());
			_isPlayerHit = 1;
			LOG_DEBUG("Player hit.");
			
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
}

/** Display and keep up to date interface strings. */
static inline void _renderInterface()
{
	static int previousLifePointsAmount = -1, previousAmmunitionAmount = -1, previousSpawnersCount = -1; // Initialize everything to -1 to force the strings to be generated on game first frame
	char string[64];
	Renderer::TextColorId colorId;
	int amount;
	
	// Display background
	Renderer::renderTexture(_pointerInterfaceBackgroundTexture, CONFIGURATION_DISPLAY_HUD_BACKGROUND_TEXTURE_X, CONFIGURATION_DISPLAY_HUD_BACKGROUND_TEXTURE_Y);
	
	// Life points
	amount = pointerPlayer->getLifePointsAmount();
	if (amount != previousLifePointsAmount)
	{
		// Display life points in red if the player is near to death
		if (amount < 20) colorId = Renderer::TEXT_COLOR_ID_RED;
		// Display life points in green if the player life is full
		else if (amount == pointerPlayer->getMaximumLifePointsAmount()) colorId = Renderer::TEXT_COLOR_ID_GREEN;
		else colorId = Renderer::TEXT_COLOR_ID_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT]); // SDL_DestroyTexture() does not complain if the provided pointer is NULL, as it is on the first frame
		
		// Render the string
		sprintf(string, "Life : %d", amount);
		_pointerInterfaceStringTextures[INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
		
		previousLifePointsAmount = amount;
		LOG_DEBUG("Refreshed life points interface string.");
	}
	Renderer::renderTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_LIFE_POINTS_AMOUNT], CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X, CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y);
	
	// Ammunition count
	amount = pointerPlayer->getAmmunitionAmount();
	if (amount != previousAmmunitionAmount)
	{
		// Display ammunition in red if they are exhausted
		if (amount == 0) colorId = Renderer::TEXT_COLOR_ID_RED;
		else colorId = Renderer::TEXT_COLOR_ID_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_AMMUNITION_AMOUNT]);
		
		// Render the string
		sprintf(string, "Ammo : %d", amount);
		_pointerInterfaceStringTextures[INTERFACE_STRING_ID_AMMUNITION_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
		
		previousAmmunitionAmount = amount;
		LOG_DEBUG("Refreshed ammunition interface string.");
	}
	Renderer::renderTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_AMMUNITION_AMOUNT], CONFIGURATION_DISPLAY_HUD_AMMUNITION_X, CONFIGURATION_DISPLAY_HUD_AMMUNITION_Y);
	
	// Remaining enemies count
	amount = (int) _enemiesList.size();
	if (amount != previousSpawnersCount)
	{
		// Display enemies in green if they are all dead and all enemy spawners are destroyed
		if ((amount == 0) && (LevelManager::enemySpawnersList.empty())) colorId = Renderer::TEXT_COLOR_ID_GREEN;
		else colorId = Renderer::TEXT_COLOR_ID_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_ENEMIES_COUNT]);
		
		// Render the string
		sprintf(string, "Enemies : %d", amount);
		_pointerInterfaceStringTextures[INTERFACE_STRING_ID_ENEMIES_COUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
		
		previousSpawnersCount = amount;
		LOG_DEBUG("Refreshed enemies interface string.");
	}
	Renderer::renderTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_ENEMIES_COUNT], CONFIGURATION_DISPLAY_HUD_ENEMIES_X, CONFIGURATION_DISPLAY_HUD_ENEMIES_Y);
	
	// Display a centered message if needed
	if (_isPlayerDead) Renderer::renderCenteredTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_GAME_LOST]);
	else if (_isGameFinished) Renderer::renderCenteredTexture(_pointerInterfaceStringTextures[INTERFACE_STRING_ID_GAME_WON]);
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
	
	// Render the level walls and static objects (ammunition, medipacks, enemy spawners...)
	LevelManager::renderScene(sceneX, sceneY);
	
	// Display enemies
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator) (*enemiesListIterator)->render();
	
	// Display bullets after enemies, so when multiple enemies fire on themselves bullets are visible on top of enemies
	std::list<MovingEntityBullet *>::iterator bulletsListIterator;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	
	// Display the player now, so it is always rendered on top on everything else and can always be visible
	pointerPlayer->render();
	
	// Display special effects at the end, so they can recover everything
	EffectManager::render();
	
	// Display the red overlay
	if (_isPlayerHit)
	{
		SDL_RenderCopy(Renderer::pointerRenderer, _pointerPlayerHitOverlayTexture->getTexture(), NULL, NULL);
		_isPlayerHit = 0;
	}
	
	// Display HUD
	_renderInterface();
	
	Renderer::endRendering();
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	SDL_Event event;
	unsigned int frameStartingTime, frameElapsedTime;
	int isFullScreenEnabled = 1, levelToLoadNumber, i;
	MovingEntityBullet *pointerBullet;
	const char *pointerStringsMainMenuItems[] =
	{
		"Continue game",
		"New game",
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
	
	#if CONFIGURATION_DISPLAY_IS_FRAME_RATE_DISPLAYING_ENABLED
		unsigned int frameRateStartingTime = 0;
		int framesCount = 0;
	#endif
	
	// Check parameters
	if (argc > 1)
	{
		// Process all parameters
		for (i = 1; i < argc; i++)
		{
			// Is full screen mode requested ?
			if (strcmp("-windowed", argv[i]) == 0) isFullScreenEnabled = 0;
			else if (strcmp("-help", argv[i]) == 0)
			{
				printf("Usage : %s [-windowed] [-help].\n"
					"  -windowed : play game in windowed screen mode.\n"
					"  -help     : display this help and exit.\n"
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
	
	// Create the player now that everything is working
	pointerPlayer = new FightingEntityPlayer(0, 0); // It will be placed at the right location by the level loading function
	
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
	_pointerInterfaceStringTextures[INTERFACE_STRING_ID_GAME_LOST] =  Renderer::renderTextToTexture("You are dead !", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	_pointerInterfaceStringTextures[INTERFACE_STRING_ID_GAME_WON] = Renderer::renderTextToTexture("All levels completed. You are legend.", Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
	// Interface background
	_pointerInterfaceBackgroundTexture = getTextureFromId(TextureManager::TEXTURE_ID_GRAPHIC_USER_INTERFACE_BACKGROUND)->getTexture();
	
	LOG_INFORMATION("Game engine successfully initialized.");
	
	// Display the main menu and get user choice
	switch (Menu::display("Main menu", pointerStringsMainMenuItems, 3))
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
			break;
			
		// Start a new game (nothing to do because all needed variables are already initialized)
		case 1:
			break;
			
		default:
			goto Exit;
	}
	
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
			LOG_INFORMATION("Game paused.");
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
			
			LOG_INFORMATION("Game continuing.");
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
