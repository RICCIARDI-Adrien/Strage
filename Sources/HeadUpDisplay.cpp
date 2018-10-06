/** @file HeadUpDisplay.cpp
 * See HeadUpDisplay.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.hpp>
#include <FightingEntityPlayer.hpp>
#include <HeadUpDisplay.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

namespace HeadUpDisplay
{

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All strings the HUD can display. */
typedef enum
{
	STRING_ID_LIFE_POINTS_AMOUNT,
	STRING_ID_AMMUNITION_AMOUNT,
	STRING_ID_ENEMIES_AMOUNT,
	STRING_ID_MORTAR_STATE,
	STRING_IDS_COUNT
} StringId;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Cache all the pre-rendered string textures to avoid to render them at each frame, which is useless if they don't change. */
static SDL_Texture *_pointerStringTextures[STRING_IDS_COUNT] = {0};

//-------------------------------------------------------------------------------------------------
// Public function
//-------------------------------------------------------------------------------------------------
void setLifePointsAmount(int amount)
{
	Renderer::TextColorId colorId;
	char string[64];
	
	// Display life points in red if the player is near to death
	if (amount < 20) colorId = Renderer::TEXT_COLOR_ID_RED;
	// Display life points in green if the player life is full
	else if (amount == pointerPlayer->getMaximumLifePointsAmount()) colorId = Renderer::TEXT_COLOR_ID_GREEN;
	else colorId = Renderer::TEXT_COLOR_ID_BLUE;
	
	// Free previous string
	SDL_DestroyTexture(_pointerStringTextures[STRING_ID_LIFE_POINTS_AMOUNT]); // SDL_DestroyTexture() does not complain if the provided pointer is NULL, as it is on the first frame
	
	// Render the string
	sprintf(string, "Life : %d", amount);
	_pointerStringTextures[STRING_ID_LIFE_POINTS_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
	
	LOG_DEBUG("Refreshed life points interface string.");
}

void setAmmunitionAmount(int amount)
{
	Renderer::TextColorId colorId;
	char string[64];
	
	// Display ammunition in red if they are exhausted
	if (amount == 0) colorId = Renderer::TEXT_COLOR_ID_RED;
	else colorId = Renderer::TEXT_COLOR_ID_BLUE;
	
	// Free previous string
	SDL_DestroyTexture(_pointerStringTextures[STRING_ID_AMMUNITION_AMOUNT]);
	
	// Render the string
	sprintf(string, "Ammo : %d", amount);
	_pointerStringTextures[STRING_ID_AMMUNITION_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
	
	LOG_DEBUG("Refreshed ammunition interface string.");
}

void setEnemiesAmount(int amount)
{
	Renderer::TextColorId colorId;
	char string[64];
	
	// Display enemies in green if they are all dead and all enemy spawners are destroyed
	if ((amount == 0) && (LevelManager::enemySpawnersList.empty())) colorId = Renderer::TEXT_COLOR_ID_GREEN;
	else colorId = Renderer::TEXT_COLOR_ID_BLUE;
	
	// Free previous string
	SDL_DestroyTexture(_pointerStringTextures[STRING_ID_ENEMIES_AMOUNT]);
	
	// Render the string
	sprintf(string, "Enemies : %d", amount);
	_pointerStringTextures[STRING_ID_ENEMIES_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
	
	LOG_DEBUG("Refreshed enemies interface string.");
}

void render()
{
	Renderer::renderTexture(_pointerStringTextures[STRING_ID_LIFE_POINTS_AMOUNT], CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X, CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y);
	Renderer::renderTexture(_pointerStringTextures[STRING_ID_AMMUNITION_AMOUNT], CONFIGURATION_DISPLAY_HUD_AMMUNITION_X, CONFIGURATION_DISPLAY_HUD_AMMUNITION_Y);
	Renderer::renderTexture(_pointerStringTextures[STRING_ID_ENEMIES_AMOUNT], CONFIGURATION_DISPLAY_HUD_ENEMIES_X, CONFIGURATION_DISPLAY_HUD_ENEMIES_Y);
}

}
