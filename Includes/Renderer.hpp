/** @file Renderer.hpp
 * The game window renderer.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_RENDERER_HPP
#define HPP_RENDERER_HPP

#include <SDL2/SDL.h>

namespace Renderer
{

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The renderer used to render to the game window. */
extern SDL_Renderer *pointerMainRenderer;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the SDL2 library and create a window with a renderer.
 * @return 0 if all went successful,
 * @return -1 if an error occurred.
 */
int initialize();

/** Free all allocated resources.
 * @note This function must be called after all SDL resources have been freed.
 */
void uninitialize();

}

#endif
