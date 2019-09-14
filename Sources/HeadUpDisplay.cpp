/** @file HeadUpDisplay.cpp
 * See HeadUpDisplay.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <Configuration.hpp>
#include <HeadUpDisplay.hpp>
#include <LevelManager.hpp>
#include <Log.hpp>
#include <PlayerFightingEntity.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <TextureManager.hpp>

namespace HeadUpDisplay
{
	/** All strings the HUD can display. */
	typedef enum
	{
		STRING_ID_LIFE_POINTS_AMOUNT,
		STRING_ID_AMMUNITION_AMOUNT,
		STRING_ID_ENEMIES_AMOUNT,
		STRING_ID_MORTAR_STATE,
		STRING_ID_REMAINING_BONUS_TIME,
		STRING_IDS_COUNT
	} StringId;
	
	/** Display a specific arrow at a specific screen position. */
	typedef struct
	{
		Texture *pointerTurnedOffTexture; //!< The texture to display when the arrow is not lighted.
		Texture *pointerLightedTexture; //!< The texture to display when the arrow is lighted.
		int x; //!< The texture displaying horizontal coordinate.
		int y; //!< The texture displaying vertical coordinate.
		bool isLighted; //!< Display lighted texture when true, display turned off texture when false.
	} CompassArrow;

	/** Cache all the pre-rendered string textures to avoid to render them at each frame, which is useless if they don't change. */
	static SDL_Texture *_pointerStringTextures[STRING_IDS_COUNT] = {0};

	/** Point to the interface background texture with the right type for the rendering function. */
	static SDL_Texture *_pointerBackgroundTexture;

	/** Tell whether bonus remaining time must be displayed. */
	static bool _isBonusRemainingSecondsStringDisplayed = false;

	/** All compass arrows. */
	static CompassArrow _compassArrows[COMPASS_ARROW_IDS_COUNT];

	/** Display player life amount. */
	static inline void _displayPlayerLifePointsAmount()
	{
		// Check whether the string must be rendered
		static int previousLifePointsAmount = -10; // This value can't be reached in game, so the string displaying is forced on game start
		int lifePointsAmount = pointerPlayer->getLifePointsAmount();
		if (lifePointsAmount != previousLifePointsAmount)
		{
			// Display life points in red if the player is near to death
			Renderer::TextColorId colorId;
			if (lifePointsAmount < 20) colorId = Renderer::TEXT_COLOR_ID_RED;
			// Display life points in green if the player life is full
			else if (lifePointsAmount == pointerPlayer->getMaximumLifePointsAmount()) colorId = Renderer::TEXT_COLOR_ID_GREEN;
			else colorId = Renderer::TEXT_COLOR_ID_LIGHT_BLUE;
			
			// Free previous string
			SDL_DestroyTexture(_pointerStringTextures[STRING_ID_LIFE_POINTS_AMOUNT]); // SDL_DestroyTexture() does not complain if the provided pointer is NULL, as it is on the first frame
			
			// Render the string
			char string[64];
			sprintf(string, "Life : %d", lifePointsAmount);
			_pointerStringTextures[STRING_ID_LIFE_POINTS_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
			
			previousLifePointsAmount = lifePointsAmount;
			LOG_DEBUG("Refreshed life points interface string.");
		}
		
		// Render the string
		Renderer::renderTexture(_pointerStringTextures[STRING_ID_LIFE_POINTS_AMOUNT], CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X, CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y);
	}

	static inline void _displayPlayerAmmunitionAmount()
	{
		// Check whether the string must be rendered
		static int previousAmmunitionAmount = -10; // This value can't be reached in game, so the string displaying is forced on game start
		int ammunitionAmount = pointerPlayer->getAmmunitionAmount();
		if (ammunitionAmount != previousAmmunitionAmount)
		{
			// Display ammunition in red if they are exhausted
			Renderer::TextColorId colorId;
			if (ammunitionAmount == 0) colorId = Renderer::TEXT_COLOR_ID_RED;
			else colorId = Renderer::TEXT_COLOR_ID_LIGHT_BLUE;
			
			// Free previous string
			SDL_DestroyTexture(_pointerStringTextures[STRING_ID_AMMUNITION_AMOUNT]);
			
			// Render the string
			char string[64];
			sprintf(string, "Ammo : %d", ammunitionAmount);
			_pointerStringTextures[STRING_ID_AMMUNITION_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
			
			previousAmmunitionAmount = ammunitionAmount;
			LOG_DEBUG("Refreshed ammunition interface string.");
		}
		
		// Render the string
		Renderer::renderTexture(_pointerStringTextures[STRING_ID_AMMUNITION_AMOUNT], CONFIGURATION_DISPLAY_HUD_AMMUNITION_X, CONFIGURATION_DISPLAY_HUD_AMMUNITION_Y);
	}

	int initialize()
	{
		// Cache background texture access to avoid searching for it at each frame
		_pointerBackgroundTexture = getTextureFromId(TextureManager::TEXTURE_ID_HEAD_UP_DISPLAY_BACKGROUND)->getSDLTexture();
		
		// Cache compass textures and position
		// Down arrow
		_compassArrows[1].pointerTurnedOffTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_DOWN_TURNED_OFF);
		_compassArrows[1].pointerLightedTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_DOWN_LIGHTED);
		_compassArrows[1].x = (Renderer::displayWidth - _compassArrows[1].pointerTurnedOffTexture->getWidth()) / 2;
		_compassArrows[1].y = Renderer::displayHeight - _compassArrows[1].pointerTurnedOffTexture->getHeight() - 20; // Start from downer arrow as it has a fixed vertical position from the screen bottom
		// Left arrow
		_compassArrows[2].pointerTurnedOffTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_LEFT_TURNED_OFF);
		_compassArrows[2].pointerLightedTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_LEFT_LIGHTED);
		_compassArrows[2].x = ((Renderer::displayWidth - _compassArrows[2].pointerTurnedOffTexture->getWidth()) / 2) - 42;
		_compassArrows[2].y = _compassArrows[1].y - _compassArrows[2].pointerTurnedOffTexture->getHeight() + 8; // Left arrow vertical position is computed from down arrow position
		// Right arrow
		_compassArrows[3].pointerTurnedOffTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_RIGHT_TURNED_OFF);
		_compassArrows[3].pointerLightedTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_RIGHT_LIGHTED);
		_compassArrows[3].x = ((Renderer::displayWidth - _compassArrows[2].pointerTurnedOffTexture->getWidth()) / 2) + 42;
		_compassArrows[3].y = _compassArrows[2].y; // Right arrow vertical position is the same than left arrow
		// Up arrow
		_compassArrows[0].pointerTurnedOffTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_UP_TURNED_OFF);
		_compassArrows[0].pointerLightedTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_COMPASS_ARROW_UP_LIGHTED);
		_compassArrows[0].x = (Renderer::displayWidth - _compassArrows[0].pointerTurnedOffTexture->getWidth()) / 2;
		_compassArrows[0].y = _compassArrows[2].y - _compassArrows[0].pointerTurnedOffTexture->getHeight() + 8;

		return 0;
	}

	void setEnemiesAmount(int amount)
	{
		// Display enemies in green if they are all dead and all enemy spawners are destroyed
		Renderer::TextColorId colorId;
		if ((amount == 0) && (LevelManager::enemySpawnersList.empty())) colorId = Renderer::TEXT_COLOR_ID_GREEN;
		else colorId = Renderer::TEXT_COLOR_ID_LIGHT_BLUE;
		
		// Free previous string
		SDL_DestroyTexture(_pointerStringTextures[STRING_ID_ENEMIES_AMOUNT]);
		
		// Render the string
		char string[64];
		sprintf(string, "Enemies : %d", amount);
		_pointerStringTextures[STRING_ID_ENEMIES_AMOUNT] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
		
		LOG_DEBUG("Refreshed enemies interface string.");
	}

	void setMortarState(MortarState state)
	{
		static MortarState previousMortarState = MORTAR_STATE_INVALID; // Initialize with an invalid value to make sure the first string is generated
		
		// Nothing to do if the string has been rendered yet
		if (state == previousMortarState) return;
		
		// Add string prefix
		char string[64];
		strcpy(string, "Mortar : ");
		
		// Add string suffix and select color
		Renderer::TextColorId colorId;
		switch (state)
		{
			case MORTAR_STATE_LOW_AMMUNITION:
				colorId = Renderer::TEXT_COLOR_ID_RED;
				strcat(string, "low ammo");
				break;
				
			case MORTAR_STATE_RELOADING:
				colorId = Renderer::TEXT_COLOR_ID_LIGHT_BLUE;
				strcat(string, "reloading...");
				break;
				
			case MORTAR_STATE_READY:
				colorId = Renderer::TEXT_COLOR_ID_GREEN;
				strcat(string, "ready");
				break;
				
			default:
				LOG_ERROR("Invalid mortar state : %d.", state);
				exit(-1);
				break;
		}
		
		// Render the string
		_pointerStringTextures[STRING_ID_MORTAR_STATE] = Renderer::renderTextToTexture(string, colorId, Renderer::FONT_SIZE_ID_SMALL);
		
		previousMortarState = state;
	}

	void setRemainingBonusTime(int timeSeconds)
	{
		if (timeSeconds == 0)
		{
			_isBonusRemainingSecondsStringDisplayed = false;
			return;
		}
		
		// Free previous string
		SDL_DestroyTexture(_pointerStringTextures[STRING_ID_REMAINING_BONUS_TIME]);
		
		// Render the string
		char string[64];
		sprintf(string, "Bonus time : %d", timeSeconds);
		_pointerStringTextures[STRING_ID_REMAINING_BONUS_TIME] = Renderer::renderTextToTexture(string, Renderer::TEXT_COLOR_ID_BLUE, Renderer::FONT_SIZE_ID_BIG);
		
		_isBonusRemainingSecondsStringDisplayed = true;
		
		LOG_DEBUG("Refreshed bonus timer interface string.");
	}

	void setCompassArrowState(CompassArrowId id, bool isLighted)
	{
		assert(id < COMPASS_ARROW_IDS_COUNT);
		_compassArrows[id].isLighted = isLighted;
	}

	void render()
	{
		// Display background
		Renderer::renderTexture(_pointerBackgroundTexture, CONFIGURATION_DISPLAY_HUD_BACKGROUND_TEXTURE_X, CONFIGURATION_DISPLAY_HUD_BACKGROUND_TEXTURE_Y);
		
		// Display HUD content
		_displayPlayerLifePointsAmount();
		_displayPlayerAmmunitionAmount();
		Renderer::renderTexture(_pointerStringTextures[STRING_ID_ENEMIES_AMOUNT], CONFIGURATION_DISPLAY_HUD_ENEMIES_X, CONFIGURATION_DISPLAY_HUD_ENEMIES_Y);
		Renderer::renderTexture(_pointerStringTextures[STRING_ID_MORTAR_STATE], CONFIGURATION_DISPLAY_HUD_MORTAR_STATE_X, CONFIGURATION_DISPLAY_HUD_MORTAR_STATE_Y);
		
		// Display bonus timer (if any)
		if (_isBonusRemainingSecondsStringDisplayed) Renderer::renderCenteredTexture(_pointerStringTextures[STRING_ID_REMAINING_BONUS_TIME], CONFIGURATION_DISPLAY_HUD_BONUS_TIMER_Y);
		
		// Display compass
		for (int i = 0; i < COMPASS_ARROW_IDS_COUNT; i++)
		{
			if (_compassArrows[i].isLighted) _compassArrows[i].pointerLightedTexture->render(_compassArrows[i].x, _compassArrows[i].y);
			else  _compassArrows[i].pointerTurnedOffTexture->render(_compassArrows[i].x, _compassArrows[i].y);
		}
	}
}
