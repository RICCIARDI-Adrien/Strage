/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <cstdlib>
#include <ctime>
#include <EntityAnimatedTexture.hpp>
#include <EntityAnimatedTextureEnemySpawnerExplosion.hpp>
#include <EntityAnimatedTextureShipExplosion.hpp>
#include <EntityEnemySpawner.hpp>
#include <FightingEntityEnemy.hpp>
#include <FightingEntityPlayer.hpp>
#include <list>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <MovableEntityBullet.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

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

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All bullets shot by the player. */
static std::list<MovableEntityBullet *> _playerBulletsList;
/** All bullets shot by the enemies. */
static std::list<MovableEntityBullet *> _enemiesBulletsList;

/** All enemies. */
static std::list<FightingEntityEnemy *> _enemiesList;

/** All animated textures. */
static std::list<EntityAnimatedTexture *> _animatedTexturesList;

/** How many pixels to add to spawn X coordinate to make the enemy spawn on block center. */
static int _enemySpawnOffsetX;
/** How many pixels to add to spawn Y coordinate to make the enemy spawn on block center. */
static int _enemySpawnOffsetY;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
// The variable must be outside of any namespace
FightingEntityPlayer *pointerPlayer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically free allocated resources on program shutdown. */
static void _exitFreeResources()
{
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
	if ((blockContent & LevelManager::BLOCK_CONTENT_MEDIPACK) || (blockContent & LevelManager::BLOCK_CONTENT_AMMUNITION)) return;
	
	// Select which item to spawn
	if (rand() % 2 == 0)
	{
		// Spawn a medipack
		if (rand() % 100 < CONFIGURATION_GAMEPLAY_MEDIPACK_ITEM_SPAWN_PROBABILITY_PERCENTAGE)
		{
			// Spawn the item
			blockContent |= LevelManager::BLOCK_CONTENT_MEDIPACK;
			LevelManager::setBlockContent(x, y, blockContent);
			
			LOG_DEBUG("Enemy dropped a medipack.\n");
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
			
			LOG_DEBUG("Enemy dropped ammunition.\n");
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
	int x, y;
	
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
	
	LOG_DEBUG("Spawned an enemy on map coordinates (%d, %d).\n", x, y);
	
	return new FightingEntityEnemy(x, y);
}

/** Update all game actors. */
static inline void _updateGameLogic()
{
	// Check if pickable objects can be taken by the player
	pointerPlayer->update();
	
	// Check if player bullets have hit a wall or an enemy
	std::list<MovableEntityBullet *>::iterator bulletsListIterator;
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	std::list<EntityEnemySpawner *>::iterator enemySpawnersListIterator;
	MovableEntityBullet *pointerPlayerBullet;
	FightingEntityEnemy *pointerEnemy;
	EntityEnemySpawner *pointerEnemySpawner;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator)
	{
		pointerPlayerBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerPlayerBullet->update() != 0)
		{
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
				// Remove the bullet
				bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
				
				// Wound the enemy
				pointerEnemy->modifyLife(-5); // TODO put bullet damage in bullet if more bullet types are to be created
				
				AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_BULLET_IMPACT);
				LOG_DEBUG("Enemy hit.\n");
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
				// Remove the bullet
				bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
				
				// Damage the enemy spawner
				pointerEnemySpawner->modifyLife(-10); // TODO put bullet damage in bullet if more bullet types are to be created
				
				AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_SPAWNER_BULLET_IMPACT);
				LOG_DEBUG("Enemy spawner hit.\n");
				break;
			}
		}
	}
	
	// Check if enemies bullets have hit the player
	MovableEntityBullet *pointerEnemyBullet;
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator)
	{
		pointerEnemyBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerEnemyBullet->update() != 0)
		{
			bulletsListIterator = _playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), pointerEnemyBullet->getPositionRectangle()))
		{
			// Remove the bullet
			bulletsListIterator = _enemiesBulletsList.erase(bulletsListIterator);
			
			// Wound the player
			pointerPlayer->modifyLife(-1);
			LOG_DEBUG("Player hit.\n");
			
			// TODO player dead
		}
	}
	
	// Update enemies artificial intelligence
	MovableEntityBullet *pointerBullet;
	int result;
	SDL_Rect *pointerPositionRectangle;
	for (enemiesListIterator = _enemiesList.begin(); enemiesListIterator != _enemiesList.end(); ++enemiesListIterator)
	{
		pointerEnemy = *enemiesListIterator;
		
		result = pointerEnemy->update();
		// Remove the enemy if it is dead
		if (result == 1)
		{
			enemiesListIterator = _enemiesList.erase(enemiesListIterator);
			
			// Spawn an explosion effect
			pointerPositionRectangle = pointerEnemy->getPositionRectangle();
			_animatedTexturesList.push_front(new EntityAnimatedTextureShipExplosion(pointerPositionRectangle->x, pointerPositionRectangle->y));
			AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_EXPLOSION);
			
			// Spawn an item on the current block if player is lucky
			_spawnItem(pointerPositionRectangle->x + (pointerPositionRectangle->w / 2), pointerPositionRectangle->y + (pointerPositionRectangle->h / 2)); // Use enemy center coordinates to avoid favoring one block among others
			
			continue;
		}
		// The enemy wants to shoot
		else if (result == 2)
		{
			pointerBullet = pointerEnemy->shoot();
			
			// Was the enemy allowed to fire ?
			if (pointerBullet != NULL) _enemiesBulletsList.push_front(pointerBullet);
		}
	}
	
	// Update enemy spawners at the end, to avoid new enemies being spawned in the middle of the update function
	int blockContent;
	for (enemySpawnersListIterator = LevelManager::enemySpawnersList.begin(); enemySpawnersListIterator != LevelManager::enemySpawnersList.end(); ++enemySpawnersListIterator)
	{
		pointerEnemySpawner = *enemySpawnersListIterator;
		
		result = pointerEnemySpawner->update();
		// Remove the spawner if it is destroyed
		if (result == 1)
		{
			// Remove the spawner
			enemySpawnersListIterator = LevelManager::enemySpawnersList.erase(enemySpawnersListIterator);
			
			// Remove the spawner indicator from the block
			pointerPositionRectangle = pointerEnemySpawner->getPositionRectangle();
			blockContent = LevelManager::getBlockContent(pointerPositionRectangle->x, pointerPositionRectangle->y);
			blockContent &= ~LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
			LevelManager::setBlockContent(pointerPositionRectangle->x, pointerPositionRectangle->y, blockContent);
			
			// Display an explosion
			_animatedTexturesList.push_front(new EntityAnimatedTextureEnemySpawnerExplosion(pointerPositionRectangle->x, pointerPositionRectangle->y));
			AudioManager::playSound(AudioManager::SOUND_ID_ENEMY_SPAWNER_EXPLOSION);
		}
		// Try to spawn an enemy if the spawner requested to
		else if (result == 2)
		{
			pointerPositionRectangle = pointerEnemySpawner->getPositionRectangle();
			pointerEnemy = _spawnEnemy(pointerPositionRectangle->x, pointerPositionRectangle->y);
			if (pointerEnemy != NULL) _enemiesList.push_front(pointerEnemy);
		}
	}
	
	// Update animated textures at the end, because they can be spawned by previous updates
	std::list<EntityAnimatedTexture *>::iterator animatedTexturesListIterator;
	EntityAnimatedTexture *pointerAnimatedTexture;
	for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator)
	{
		pointerAnimatedTexture = *animatedTexturesListIterator;
		
		if (pointerAnimatedTexture->update() != 0) animatedTexturesListIterator = _animatedTexturesList.erase(animatedTexturesListIterator);
	}
}

/** Display everything to the screen.
 * @param sceneX The "camera" will render the scene starting from this scene horizontal coordinate.
 * @param sceneY The "camera" will render the scene starting from this scene vertical coordinate.
 */
static inline void _renderGame(int sceneX, int sceneY)
{
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
	std::list<MovableEntityBullet *>::iterator bulletsListIterator;
	for (bulletsListIterator = _playerBulletsList.begin(); bulletsListIterator != _playerBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	for (bulletsListIterator = _enemiesBulletsList.begin(); bulletsListIterator != _enemiesBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	
	// Display the player now, so it is always rendered on top on everything else and can always be visible
	pointerPlayer->render();
	
	// Display special effects at the end, so they can recover everything
	std::list<EntityAnimatedTexture *>::iterator animatedTexturesListIterator;
	for (animatedTexturesListIterator = _animatedTexturesList.begin(); animatedTexturesListIterator != _animatedTexturesList.end(); ++animatedTexturesListIterator) (*animatedTexturesListIterator)->render();
	
	// Display HUD
	char string[64];
	// Life points
	sprintf(string, "Life : %d%%", pointerPlayer->getLifePointsAmount());
	Renderer::renderText(CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X, CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y, string);
	// Ammunition count
	sprintf(string, "Ammo : %d", pointerPlayer->getAmmunitionAmount());
	Renderer::renderText(CONFIGURATION_DISPLAY_HUD_AMMUNITION_X, CONFIGURATION_DISPLAY_HUD_AMMUNITION_Y, string);
	
	// Display the rendered picture
	SDL_RenderPresent(Renderer::pointerMainRenderer);
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	SDL_Event event;
	unsigned int Starting_Time, Elapsed_Time;
	int isKeyPressed[KEYBOARD_KEY_IDS_COUNT] = {0}, isLastDirectionVertical;
	
	// Engine initialization
	if (Renderer::initialize() != 0) return -1;
	if (TextureManager::initialize() != 0) return -1;
	if (LevelManager::initialize() != 0) return -1;
	if (AudioManager::initialize() != 0) return -1;
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
	atexit(_exitFreeResources);
	
	// Cache some values
	_enemySpawnOffsetX = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY)->getWidth() / 2;
	_enemySpawnOffsetY = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY)->getHeight() / 2;
	
	// Initialize pseudo-random numbers generator
	srand(time(NULL));
	
	LOG_INFORMATION("Game engine successfully initialized.\n");
	
	// TEST
	int camX = 0, camY = 0;
	LevelManager::loadLevel("Levels/Test_Scene.csv", "Levels/Test_Objects.csv");
	
	while (1)
	{
		// Store the time when the loop started
		Starting_Time = SDL_GetTicks();
		
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
			MovableEntityBullet *pointerBullet = pointerPlayer->shoot();
			
			// Is the player allowed to shoot ?
			if (pointerBullet != NULL) _playerBulletsList.push_front(pointerBullet);
		}
		
		// TEST
		camX = pointerPlayer->getX() - (CONFIGURATION_DISPLAY_WIDTH / 2) + 16;
		camY = pointerPlayer->getY() - (CONFIGURATION_DISPLAY_HEIGHT / 2) + 16;
		
		_updateGameLogic();
		
		_renderGame(camX, camY);
		
		// Wait enough time to achieve a 60Hz refresh rate
		Elapsed_Time = SDL_GetTicks() - Starting_Time;
		if (Elapsed_Time < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - Elapsed_Time);
	}
	
Exit:
	return 0;
}