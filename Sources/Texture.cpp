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
	SDL_Texture *pointerTexture = SDL_CreateTextureFromSurface(pointerMainRenderer, pointerSurface);
	SDL_FreeSurface(pointerSurface);
	
	return pointerTexture;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
Texture::Texture(const char *fileName)
{
	// Try to load the texture
	_pointerTexture = loadFromBitmap(fileName);
	if (_pointerTexture == NULL) exit(EXIT_FAILURE);
}

Texture::~Texture()
{
	SDL_DestroyTexture(_pointerTexture);
}