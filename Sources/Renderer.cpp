/** @file Renderer.cpp
 * @see Renderer.hpp for description.
 * @author Adrien RICCIARDI
 */
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

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int initialize()
{
	// Initialize the needed subsystems
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		LOG_ERROR("SDL_Init() failed (%s).\n", SDL_GetError());
		goto Exit_Error;
	}
	
	// Create the game window (fullscreen mode)
	_pointerMainWindow = SDL_CreateWindow("Strage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIGURATION_DISPLAY_WIDTH, CONFIGURATION_DISPLAY_HEIGHT, /*SDL_WINDOW_FULLSCREEN*/ 0); // TODO enable fullscreen, TODO runtime configurable resolution
	if (_pointerMainWindow == NULL)
	{
		LOG_ERROR("Failed to create the main window (%s).\n", SDL_GetError());
		goto Exit_Error_Uninitialize_SDL;
	}
	
	// Create the window renderer
	pointerMainRenderer = SDL_CreateRenderer(_pointerMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pointerMainRenderer == NULL)
	{
		LOG_ERROR("Failed to create the main renderer (%s).\n", SDL_GetError());
		goto Exit_Error_Destroy_Window;
	}
	
	// Initialize SDL TTF
	if (TTF_Init() != 0)
	{
		LOG_ERROR("Failed to initialize SDL TTF library (%s).\n", TTF_GetError());
		goto Exit_Error_Destroy_Renderer;
	}
	
	// Try to load the font
	_pointerFont = TTF_OpenFont("Liberation_Sans_Bold.ttf", 36);
	if (_pointerFont == NULL)
	{
		LOG_ERROR("Failed to load TTF font file (%s).\n", TTF_GetError());
		goto Exit_Error_Uninitialize_TTF;
	}
	
	// Cache some values
	_displayRectangle.w = CONFIGURATION_DISPLAY_WIDTH;
	_displayRectangle.h = CONFIGURATION_DISPLAY_HEIGHT;
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

void renderText(int x, int y, const char *stringText)
{
	// Render the text
	//SDL_Surface *pointerSurface = TTF_RenderText_Solid(_pointerFont, stringText, _textRenderingColor);
	SDL_Surface *pointerSurface = TTF_RenderText_Blended(_pointerFont, stringText, _textRenderingColor);
	if (pointerSurface == NULL)
	{
		LOG_ERROR("Failed to render the text to a surface, shutting down (%s)\n.", TTF_GetError());
		exit(-1);
	}
	
	// Convert it to a texture to be able to display it
	SDL_Texture *pointerTexture = SDL_CreateTextureFromSurface(pointerMainRenderer, pointerSurface);
	SDL_FreeSurface(pointerSurface);
	if (pointerTexture == NULL)
	{
		LOG_ERROR("Failed to convert the surface to a texture, shutting down (%s)\n.", SDL_GetError());
		exit(-1);
	}
	
	// Get the texture dimensions
	SDL_Rect destinationRectangle;
	unsigned int pixelFormat;
	int access;
	if (SDL_QueryTexture(pointerTexture, &pixelFormat, &access, &destinationRectangle.w, &destinationRectangle.h) != 0)
	{
		LOG_ERROR("Failed to query information about the texture, shutting down (%s)\n.", SDL_GetError());
		exit(-1);
	}
	
	// Display the texture at the specified coordinates
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	SDL_RenderCopy(pointerMainRenderer, pointerTexture, NULL, &destinationRectangle);
}

}
