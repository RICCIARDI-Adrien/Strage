/** @file Texture.cpp
 * @see Texture.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>
#include <Texture.hpp>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
SDL_Texture *Texture::loadFromBitmap(const char *fileName)
{
	// Try to load bitmap
	SDL_Surface *pointerSurface = SDL_LoadBMP(fileName);
	if (pointerSurface == NULL)
	{
		LOG("Error : failed to load texture '%s' (%s).\n", fileName, SDL_GetError());
		return NULL;
	}
	
	// Set the transparent color as bright green (lime)
	unsigned int transparentColor = SDL_MapRGB(pointerSurface->format, 0x00, 0xFF, 0x00);
	if (SDL_SetColorKey(pointerSurface, SDL_TRUE, transparentColor) != 0)
	{
		LOG("Error : failed to set the surface transparent color (%s).\n", SDL_GetError());
		return NULL;
	}
	
	// Convert the surface to a texture
	SDL_Texture *pointerTexture = SDL_CreateTextureFromSurface(Renderer::pointerMainRenderer, pointerSurface);
	SDL_FreeSurface(pointerSurface);
	if (pointerTexture == NULL)
	{
		LOG("Error : failed to convert the surface to a texture (%s).\n", SDL_GetError());
		return NULL;
	}
	
	return pointerTexture;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
Texture::Texture(const char *fileName)
{
	unsigned int pixelFormat;
	int access;
	
	// Try to load the texture
	_pointerTexture = loadFromBitmap(fileName);
	if (_pointerTexture == NULL) exit(-1);
	
	// Cache width and height parameters
	if (SDL_QueryTexture(_pointerTexture, &pixelFormat, &access, &_width, &_height) != 0)
	{
		LOG("Error : failed to query texture information (%s).\n", SDL_GetError());
		exit(-1);
	}
}

Texture::~Texture()
{
	SDL_DestroyTexture(_pointerTexture);
}

void Texture::render(int x, int y)
{
	SDL_Rect destinationRectangle;
	
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	destinationRectangle.w = _width;
	destinationRectangle.h = _height;
	
	SDL_RenderCopy(Renderer::pointerMainRenderer, _pointerTexture, NULL, &destinationRectangle);
}
