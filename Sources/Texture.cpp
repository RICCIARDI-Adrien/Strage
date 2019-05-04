/** @file Texture.cpp
 * See Texture.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <Log.hpp>
#include <Renderer.hpp>
#include <Texture.hpp>

Texture::Texture(SDL_Texture *pointerSDLTexture)
{
	// Cache width and height parameters
	_pointerSDLTexture = pointerSDLTexture;
	unsigned int pixelFormat;
	int access;
	if (SDL_QueryTexture(_pointerSDLTexture, &pixelFormat, &access, &_width, &_height) != 0)
	{
		LOG_ERROR("Failed to query texture information (%s).", SDL_GetError());
		exit(-1);
	}
}

int Texture::render(int x, int y)
{
	SDL_Rect positionRectangle;
	positionRectangle.x = x;
	positionRectangle.y = y;
	positionRectangle.w = _width;
	positionRectangle.h = _height;
	
	// Render texture only if it is visible on the screen
	if ((x + _width >= 0) && (y + _height >= 0) && (x < Renderer::displayWidth) && (y < Renderer::displayHeight)) SDL_RenderCopy(Renderer::pointerRenderer, _pointerSDLTexture, NULL, &positionRectangle);
	
	return 0;
}
