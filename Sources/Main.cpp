/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <ctime>
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
static std::list<MovableEntityBullet *> playerBulletsList;
/** All bullets shot by the enemies. */
static std::list<MovableEntityBullet *> enemiesBulletsList;

/** All enemies. */
static std::list<FightingEntityEnemy *> enemiesList;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
// The variable must be outside of any namespace
FightingEntityPlayer *pointerPlayer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically free allocated resources on program shutdown. */
static void exitFreeResources()
{
	LevelManager::uninitialize();
	TextureManager::uninitialize();
	Renderer::uninitialize(); // Must be called at the end because it stops SDL
}

/** Spawn a random item (or nothing) on the block into which coordinates are contained.
 * @param x X coordinate in pixels.
 * @param y Y coordinate in pixels.
 */
static void spawnItem(int x, int y)
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

/** Update all game actors. */
static void updateGameLogic()
{
	// Check if pickable objects can be taken by the player
	pointerPlayer->update();
	
	// Check if player bullets have hit a wall or an enemy
	std::list<MovableEntityBullet *>::iterator bulletsListIterator;
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	MovableEntityBullet *pointerPlayerBullet;
	FightingEntityEnemy *pointerEnemy;
	for (bulletsListIterator = playerBulletsList.begin(); bulletsListIterator != playerBulletsList.end(); ++bulletsListIterator)
	{
		pointerPlayerBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerPlayerBullet->update() != 0)
		{
			bulletsListIterator = playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		// Check with all enemies for a collision
		for (enemiesListIterator = enemiesList.begin(); enemiesListIterator != enemiesList.end(); ++enemiesListIterator)
		{
			pointerEnemy = *enemiesListIterator;
			
			// Did the bullet hit this enemy ?
			if (SDL_HasIntersection(pointerPlayerBullet->getPositionRectangle(), pointerEnemy->getPositionRectangle()))
			{
				// Remove the bullet
				bulletsListIterator = playerBulletsList.erase(bulletsListIterator);
				
				// Wound the enemy
				pointerEnemy->modifyLife(-5); // TODO put bullet damage in bullet if more bullet types are to be created
				LOG_DEBUG("Enemy hit.\n");
				
				break;
			}
		}
	}
	
	// Check if enemies bullets have hit the player
	MovableEntityBullet *pointerEnemyBullet;
	for (bulletsListIterator = enemiesBulletsList.begin(); bulletsListIterator != enemiesBulletsList.end(); ++bulletsListIterator)
	{
		pointerEnemyBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerEnemyBullet->update() != 0)
		{
			bulletsListIterator = playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), pointerEnemyBullet->getPositionRectangle()))
		{
			// Remove the bullet
			bulletsListIterator = enemiesBulletsList.erase(bulletsListIterator);
			
			// Wound the player
			pointerPlayer->modifyLife(-1);
			LOG_DEBUG("Player hit.\n");
			
			// TODO player dead
		}
	}
	
	// Update enemies artificial intelligence
	MovableEntityBullet *pointerBullet;
	int result;
	SDL_Rect *pointerEnemyPositionRectangle;
	for (enemiesListIterator = enemiesList.begin(); enemiesListIterator != enemiesList.end(); ++enemiesListIterator)
	{
		pointerEnemy = *enemiesListIterator;
		
		result = pointerEnemy->update();
		// Remove the enemy if it is dead
		if (result == 1)
		{
			enemiesListIterator = enemiesList.erase(enemiesListIterator);
			
			// TODO spawn explosion effect
			
			// Spawn an item on the current block if player is lucky
			pointerEnemyPositionRectangle = pointerEnemy->getPositionRectangle();
			spawnItem(pointerEnemyPositionRectangle->x + (pointerEnemyPositionRectangle->w / 2), pointerEnemyPositionRectangle->y + (pointerEnemyPositionRectangle->h / 2)); // Use enemy center coordinates to avoid favoring one block among others
			
			continue;
		}
		// The enemy wants to shoot
		else if (result == 2)
		{
			pointerBullet = pointerEnemy->shoot();
			
			// Was the enemy allowed to fire ?
			if (pointerBullet != NULL) enemiesBulletsList.push_front(pointerBullet);
		}
	}
}

/** Display everything to the screen.
 * @param sceneX The "camera" will render the scene starting from this scene horizontal coordinate.
 * @param sceneY The "camera" will render the scene starting from this scene vertical coordinate.
 */
static void renderGame(int sceneX, int sceneY)
{
	// Start rendering
	Renderer::beginRendering(sceneX, sceneY);
	
	// Render the level walls
	LevelManager::renderScene(sceneX, sceneY);
	
	// Display enemies
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	for (enemiesListIterator = enemiesList.begin(); enemiesListIterator != enemiesList.end(); ++enemiesListIterator) (*enemiesListIterator)->render();
	
	// Display bullets after enemies, so when multiple enemies fire on themselves bullets are visible on top of enemies
	std::list<MovableEntityBullet *>::iterator bulletsListIterator;
	for (bulletsListIterator = playerBulletsList.begin(); bulletsListIterator != playerBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	for (bulletsListIterator = enemiesBulletsList.begin(); bulletsListIterator != enemiesBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	
	// Display the player at the end, so it is always rendered on top on everything else and can always be visible
	pointerPlayer->render();
	
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
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
	atexit(exitFreeResources);
	LOG_INFORMATION("Game engine successfully initialized.\n");
	
	// Initialize pseudo-random numbers generator
	srand(time(NULL));
	
	// TEST
	int camX = 0, camY = 0;
	LevelManager::loadLevel("Levels/Test_Scene.csv", "Levels/Test_Objects.csv");
	FightingEntityEnemy e1(64*8 + 13, 64 * 3 + 35);
	FightingEntityEnemy e2(64*13 + 13, 64 * 3 + 22);
	FightingEntityEnemy e3(64*13 + 13, 64 * 4 + 22);
	FightingEntityEnemy e4(64*13 + 13, 64 * 5 + 22);
	FightingEntityEnemy e5(64*12 + 13, 64 * 5 + 22);
	FightingEntityEnemy e6(64*12 + 13, 64 * 4 + 22);
	enemiesList.push_front(&e1);
	enemiesList.push_front(&e2);
	enemiesList.push_front(&e3);
	enemiesList.push_front(&e4);
	enemiesList.push_front(&e5);
	enemiesList.push_front(&e6);
	
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
			if (pointerBullet != NULL) playerBulletsList.push_front(pointerBullet);
		}
		
		// TEST
		camX = pointerPlayer->getX() - (CONFIGURATION_DISPLAY_WIDTH / 2) + 16;
		camY = pointerPlayer->getY() - (CONFIGURATION_DISPLAY_HEIGHT / 2) + 16;
		
		updateGameLogic();
		
		renderGame(camX, camY);
		
		// Wait enough time to achieve a 60Hz refresh rate
		Elapsed_Time = SDL_GetTicks() - Starting_Time;
		if (Elapsed_Time < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - Elapsed_Time);
	}
	
Exit:
	return 0;
}