/** @file Renderer.cpp
 * See Renderer.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cassert>
#include <Configuration.hpp>
#include <cstdlib>
#include <FileManager.hpp>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#ifdef CONFIGURATION_BUILD_FOR_MACOS
	#include <SDL2_ttf/SDL_ttf.h>
#else
	#include <SDL2/SDL_ttf.h>
#endif

namespace Renderer
{
	/** The game window. */
	static SDL_Window *_pointerMainWindow;

	/** The fonts used to draw text. */
	static TTF_Font *_pointerFonts[FONT_SIZE_IDS_COUNT];
	/** All available text colors. */
	static SDL_Color _textColors[TEXT_COLOR_IDS_COUNT];
	
	/** Store the time in milliseconds when a frame creation started. */
	static unsigned int _frameStartingTime = 0;

	// Public variable, documentation is in the header file
	SDL_Renderer *pointerRenderer;

	// Public variables, documentation is in the header file
	int displayX;
	int displayY;

	// Public variables, documentation is in the header file
	int displayWidth;
	int displayHeight;

	int initialize(bool isFullScreenEnabled)
	{
		unsigned int flags;
		SDL_RendererInfo rendererInformation;
		
		// Create the game window
		if (isFullScreenEnabled) flags = SDL_WINDOW_FULLSCREEN_DESKTOP; // Automatically adjust renderer resolution to the display one
		else flags = 0;
		_pointerMainWindow = SDL_CreateWindow("Strage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, flags);
		if (_pointerMainWindow == NULL)
		{
			LOG_ERROR("Failed to create the main window (%s).", SDL_GetError());
			goto Exit_Error;
		}
		
		// Create the window renderer
		pointerRenderer = SDL_CreateRenderer(_pointerMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (pointerRenderer == NULL)
		{
			LOG_ERROR("Failed to create the main renderer (%s).", SDL_GetError());
			goto Exit_Error_Destroy_Window;
		}
		
		// Display rendering driver name
		if (SDL_GetRendererInfo(pointerRenderer, &rendererInformation) == 0) LOG_DEBUG("Rendering driver : %s.", rendererInformation.name);
		
		// Set display size according to selected mode
		if (isFullScreenEnabled)
		{
			if (SDL_GetRendererOutputSize(pointerRenderer, &displayWidth, &displayHeight) != 0)
			{
				LOG_ERROR("Failed to get renderer output size (%s).", SDL_GetError());
				goto Exit_Error_Destroy_Window;
			}
		}
		else
		{
			displayWidth = CONFIGURATION_DISPLAY_WIDTH;
			displayHeight = CONFIGURATION_DISPLAY_HEIGHT;
		}
		
		// Initialize SDL TTF
		if (TTF_Init() != 0)
		{
			LOG_ERROR("Failed to initialize SDL TTF library (%s).", TTF_GetError());
			goto Exit_Error_Destroy_Renderer;
		}
		
		// Try to load the fonts
		_pointerFonts[0] = TTF_OpenFont(FileManager::getFilePath("Liberation_Sans_Bold.ttf"), 20);
		if (_pointerFonts[0] == NULL)
		{
			LOG_ERROR("Failed to load TTF small font file (%s).", TTF_GetError());
			goto Exit_Error_Uninitialize_TTF;
		}
		_pointerFonts[1] = TTF_OpenFont(FileManager::getFilePath("Liberation_Sans_Bold.ttf"), 36);
		if (_pointerFonts[1] == NULL)
		{
			LOG_ERROR("Failed to load TTF big font file (%s).", TTF_GetError());
			goto Exit_Error_Uninitialize_TTF;
		}
		
		// Hide the cursor in full screen mode
		if (isFullScreenEnabled)
		{
			if (SDL_ShowCursor(SDL_DISABLE) < 0)
			{
				LOG_ERROR("Failed to hide cursor in full screen mode (%s).", SDL_GetError());
				goto Exit_Error_Uninitialize_TTF;
			}
		}
		
		// Cache some values
		// Black
		_textColors[TEXT_COLOR_ID_BLACK].r = 0;
		_textColors[TEXT_COLOR_ID_BLACK].g = 0;
		_textColors[TEXT_COLOR_ID_BLACK].b = 0;
		_textColors[TEXT_COLOR_ID_BLACK].a = 255;
		// Blue
		_textColors[TEXT_COLOR_ID_BLUE].r = 0;
		_textColors[TEXT_COLOR_ID_BLUE].g = 0;
		_textColors[TEXT_COLOR_ID_BLUE].b = 255;
		_textColors[TEXT_COLOR_ID_BLUE].a = 255;
		// Light blue
		_textColors[TEXT_COLOR_ID_LIGHT_BLUE].r = 54;
		_textColors[TEXT_COLOR_ID_LIGHT_BLUE].g = 194;
		_textColors[TEXT_COLOR_ID_LIGHT_BLUE].b = 255;
		_textColors[TEXT_COLOR_ID_LIGHT_BLUE].a = 255;
		// Dark green
		_textColors[TEXT_COLOR_ID_DARK_GREEN].r = 27;
		_textColors[TEXT_COLOR_ID_DARK_GREEN].g = 116;
		_textColors[TEXT_COLOR_ID_DARK_GREEN].b = 36;
		_textColors[TEXT_COLOR_ID_DARK_GREEN].a = 255;
		// Green
		_textColors[TEXT_COLOR_ID_GREEN].r = 0;
		_textColors[TEXT_COLOR_ID_GREEN].g = 200;
		_textColors[TEXT_COLOR_ID_GREEN].b = 0;
		_textColors[TEXT_COLOR_ID_GREEN].a = 255;
		// Red
		_textColors[TEXT_COLOR_ID_RED].r = 255;
		_textColors[TEXT_COLOR_ID_RED].g = 0;
		_textColors[TEXT_COLOR_ID_RED].b = 0;
		_textColors[TEXT_COLOR_ID_RED].a = 255;
		// Light grey
		_textColors[TEXT_COLOR_LIGHT_GREY].r = 224;
		_textColors[TEXT_COLOR_LIGHT_GREY].g = 224;
		_textColors[TEXT_COLOR_LIGHT_GREY].b = 224;
		_textColors[TEXT_COLOR_LIGHT_GREY].a = 255;
		// Dark grey
		_textColors[TEXT_COLOR_DARK_GREY].r = 47;
		_textColors[TEXT_COLOR_DARK_GREY].g = 47;
		_textColors[TEXT_COLOR_DARK_GREY].b = 47;
		_textColors[TEXT_COLOR_DARK_GREY].a = 255;
		
		// Everything went fine
		return 0;
		
	Exit_Error_Uninitialize_TTF:
		TTF_Quit();
		
	Exit_Error_Destroy_Renderer:
		SDL_DestroyRenderer(pointerRenderer);

	Exit_Error_Destroy_Window:
		SDL_DestroyWindow(_pointerMainWindow);
		
	Exit_Error:
		return -1;
	}

	void uninitialize()
	{
		for (int i = 0; i < FONT_SIZE_IDS_COUNT; i++) TTF_CloseFont(_pointerFonts[i]);
		TTF_Quit();
		
		SDL_DestroyRenderer(pointerRenderer);
		SDL_DestroyWindow(_pointerMainWindow);
		SDL_Quit();
	}

	void beginFrame()
	{
		// Store the time when the frame computation started
		_frameStartingTime = SDL_GetTicks();
		
		// Clean the rendering area
		SDL_RenderClear(pointerRenderer);
	}

	void endFrame()
	{
		// Display the rendered picture
		SDL_RenderPresent(pointerRenderer);

		// Wait enough time to achieve a 60Hz refresh rate
		unsigned int frameElapsedTime = SDL_GetTicks() - _frameStartingTime;
		if (frameElapsedTime < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - frameElapsedTime);
	}

	SDL_Texture *renderTextToTexture(const char *pointerStringText, TextColorId colorId, FontSizeId fontSizeId)
	{
		// Make sure parameters are valid
		assert(colorId < TEXT_COLOR_IDS_COUNT);
		assert(fontSizeId < FONT_SIZE_IDS_COUNT);
		
		// Render the text
		SDL_Surface *pointerSurface = TTF_RenderText_Blended(_pointerFonts[fontSizeId], pointerStringText, _textColors[colorId]);
		if (pointerSurface == NULL)
		{
			LOG_ERROR("Failed to render the text to a surface, shutting down (%s).", TTF_GetError());
			exit(-1);
		}
		
		// Convert it to a texture to be able to display it
		SDL_Texture *pointerTexture = SDL_CreateTextureFromSurface(pointerRenderer, pointerSurface);
		SDL_FreeSurface(pointerSurface);
		if (pointerTexture == NULL)
		{
			LOG_ERROR("Failed to convert the surface to a texture, shutting down (%s).", SDL_GetError());
			exit(-1);
		}
		
		return pointerTexture;
	}

	void renderTexture(SDL_Texture *pointerTexture, int x, int y)
	{
		// Get the texture dimensions
		SDL_Rect destinationRectangle;
		if (SDL_QueryTexture(pointerTexture, NULL, NULL, &destinationRectangle.w, &destinationRectangle.h) != 0)
		{
			LOG_ERROR("Failed to query information about the SDL texture (SDL texture pointer : %p), shutting down (%s).", pointerTexture, SDL_GetError());
			exit(-1);
		}
		
		// Display the texture at the specified coordinates
		destinationRectangle.x = x;
		destinationRectangle.y = y;
		SDL_RenderCopy(pointerRenderer, pointerTexture, NULL, &destinationRectangle);
	}

	void renderCenteredTexture(SDL_Texture *pointerTexture, int y)
	{
		// Get the texture dimensions
		SDL_Rect destinationRectangle;
		if (SDL_QueryTexture(pointerTexture, NULL, NULL, &destinationRectangle.w, &destinationRectangle.h) != 0)
		{
			LOG_ERROR("Failed to query information about the texture, shutting down (%s).", SDL_GetError());
			exit(-1);
		}
		
		// Compute texture coordinates
		destinationRectangle.x = (displayWidth - destinationRectangle.w) / 2;
		if (y == -1) destinationRectangle.y = (displayHeight - destinationRectangle.h) / 2; // Automatically center on vertical axis
		else destinationRectangle.y = y;
		
		// Display the centered texture
		SDL_RenderCopy(pointerRenderer, pointerTexture, NULL, &destinationRectangle);
	}
}
