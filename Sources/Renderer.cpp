/** @file Renderer.cpp
 * @see Renderer.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.hpp>
#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Renderer
{

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The game window. */
static SDL_Window *_pointerMainWindow;

/** Define the display area. */
static SDL_Rect _displayRectangle;

/** The font used to draw text. */
static TTF_Font *_pointerFont;
/** The text rendering color. */
static SDL_Color _textRenderingColor;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
SDL_Renderer *pointerMainRenderer;

int displayX;
int displayY;

int displayWidth;
int displayHeight;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Render the provided text on a texture.
 * @param pointerText The string to render.
 * @param pointerTexture On output, contain the rendered texture (it must be destroyed after usage).
 * @param pointerTextureWidth On output, contain the texture width in pixels.
 * @param pointerTextureHeight On output, contain the texture height in pixels.
 * @warning The function stops the game if something goes wrong.
 */
static void _renderTextToTexture(const char *pointerText, SDL_Texture **pointerTexture, int *pointerTextureWidth, int *pointerTextureHeight)
{
	// Render the text
	//SDL_Surface *pointerSurface = TTF_RenderText_Solid(_pointerFont, pointerText, _textRenderingColor);
	SDL_Surface *pointerSurface = TTF_RenderText_Blended(_pointerFont, pointerText, _textRenderingColor);
	if (pointerSurface == NULL)
	{
		LOG_ERROR("Failed to render the text to a surface, shutting down (%s).", TTF_GetError());
		exit(-1);
	}
	
	// Convert it to a texture to be able to display it
	*pointerTexture = SDL_CreateTextureFromSurface(pointerMainRenderer, pointerSurface);
	SDL_FreeSurface(pointerSurface);
	if (*pointerTexture == NULL)
	{
		LOG_ERROR("Failed to convert the surface to a texture, shutting down (%s).", SDL_GetError());
		exit(-1);
	}
	
	// Get the texture dimensions
	unsigned int pixelFormat;
	int access;
	if (SDL_QueryTexture(*pointerTexture, &pixelFormat, &access, pointerTextureWidth, pointerTextureHeight) != 0)
	{
		LOG_ERROR("Failed to query information about the texture, shutting down (%s).", SDL_GetError());
		exit(-1);
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize(int isFullScreenEnabled)
{
	unsigned int flags;
	
	// Initialize the needed subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		LOG_ERROR("SDL_Init() failed (%s).", SDL_GetError());
		goto Exit_Error;
	}
	
	// Create the game window
	if (isFullScreenEnabled) flags = SDL_WINDOW_FULLSCREEN_DESKTOP; // Automatically adjust renderer resolution to display one
	else flags = 0;
	_pointerMainWindow = SDL_CreateWindow("Strage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, flags);
	if (_pointerMainWindow == NULL)
	{
		LOG_ERROR("Failed to create the main window (%s).", SDL_GetError());
		goto Exit_Error_Uninitialize_SDL;
	}
	
	// Create the window renderer
	pointerMainRenderer = SDL_CreateRenderer(_pointerMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pointerMainRenderer == NULL)
	{
		LOG_ERROR("Failed to create the main renderer (%s).", SDL_GetError());
		goto Exit_Error_Destroy_Window;
	}
	
	// Set display size according to selected mode
	if (isFullScreenEnabled)
	{
		if (SDL_GetRendererOutputSize(pointerMainRenderer, &displayWidth, &displayHeight) != 0)
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
	
	// Try to load the font
	_pointerFont = TTF_OpenFont("Liberation_Sans_Bold.ttf", 36);
	if (_pointerFont == NULL)
	{
		LOG_ERROR("Failed to load TTF font file (%s).", TTF_GetError());
		goto Exit_Error_Uninitialize_TTF;
	}
	
	// Cache some values
	_displayRectangle.w = Renderer::displayWidth;
	_displayRectangle.h = Renderer::displayHeight;
	_textRenderingColor.r = 0;
	_textRenderingColor.g = 0;
	_textRenderingColor.b = 255;
	_textRenderingColor.a = 255;
	
	// Everything went fine
	return 0;
	
Exit_Error_Uninitialize_TTF:
	TTF_Quit();
	
Exit_Error_Destroy_Renderer:
	SDL_DestroyRenderer(pointerMainRenderer);

Exit_Error_Destroy_Window:
	SDL_DestroyWindow(_pointerMainWindow);
	
Exit_Error_Uninitialize_SDL:
	SDL_Quit();

Exit_Error:
	return -1;
}

void uninitialize()
{
	TTF_CloseFont(_pointerFont);
	TTF_Quit();
	
	SDL_DestroyRenderer(pointerMainRenderer);
	SDL_DestroyWindow(_pointerMainWindow);
	SDL_Quit();
}

void beginRendering(int x, int y)
{
	// Clean the rendering area
	SDL_RenderClear(pointerMainRenderer);
	
	// Update the display location
	_displayRectangle.x = x;
	_displayRectangle.y = y;
	displayX = x;
	displayY = y;
}

int isDisplayable(SDL_Rect *pointerObjectPositionRectangle)
{
	return SDL_HasIntersection(&_displayRectangle, pointerObjectPositionRectangle);
}

void renderText(const char *pointerText, int x, int y)
{
	SDL_Texture *pointerTexture;
	SDL_Rect destinationRectangle;
	
	// Render the text on a texture
	_renderTextToTexture(pointerText, &pointerTexture, &destinationRectangle.w, &destinationRectangle.h);
	
	// Display the texture at the specified coordinates
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	SDL_RenderCopy(pointerMainRenderer, pointerTexture, NULL, &destinationRectangle);
	SDL_DestroyTexture(pointerTexture);
}

void renderCentererText(const char *pointerText)
{
	SDL_Texture *pointerTexture;
	SDL_Rect destinationRectangle;
	
	// Render the text on a texture
	_renderTextToTexture(pointerText, &pointerTexture, &destinationRectangle.w, &destinationRectangle.h);
	
	// Display the texture on the screen center
	destinationRectangle.x = (displayWidth - destinationRectangle.w) / 2;
	destinationRectangle.y = (displayHeight - destinationRectangle.h) / 2;
	SDL_RenderCopy(pointerMainRenderer, pointerTexture, NULL, &destinationRectangle);
	SDL_DestroyTexture(pointerTexture);
}

}
