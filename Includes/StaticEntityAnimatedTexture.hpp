#ifndef HPP_STATIC_ENTITY_ANIMATED_TEXTURE_HPP
#define HPP_STATIC_ENTITY_ANIMATED_TEXTURE_HPP

#include <Log.hpp>
#include <StaticEntity.hpp>
#include <TextureManager.hpp>

/** @class StaticEntityAnimatedTexture
 * Display several textures in sequence and stop.
 * @author Adrien RICCIARDI
 */
class StaticEntityAnimatedTexture: public StaticEntity
{
	private:
		/** The currently displayed texture. */
		TextureManager::TextureId _currentTextureId;
		/** Animation ends with this texture. */
		TextureManager::TextureId _finalTextureId;
		
		/** How many game engine frames to wait before displaying the next texture. */
		int _framesPerTexture;
		/** Incremented by one on each engine game, tell when to display the next texture. */
		int _framesCounter;
		
	public:
		/** Spawn an animated texture at the requested map location.
		 * @param x X coordinate on the map.
		 * @param y Y coordinate on the map.
		 * @param initialTextureId The first animation texture.
		 * @param finalTextureId The last animation texture.
		 * @param framesPerTexture How many game engine frames to wait before displaying the next texture.
		 * @note Texture ID must follow from initial to ending, because every texture in this interval will be displayed.
		 * @warning All textures must have the same dimensions, or it will lead to an unpredictable behavior.
		 */
		StaticEntityAnimatedTexture(int x, int y, TextureManager::TextureId initialTextureId, TextureManager::TextureId finalTextureId, int framesPerTexture): StaticEntity(x, y, initialTextureId) // Start animation with the first texture
		{
			_currentTextureId = initialTextureId;
			_finalTextureId = finalTextureId;
			_framesPerTexture = framesPerTexture;
			_framesCounter = 0;
		}
		
		/** Update the _pointerTexture with the right texture when time has come.
		 * @return 0 while the animation must be displayed,
		 * @return 1 when the animation is finished and the object must be destroyed.
		 */
		virtual int update()
		{
			int currentTextureId;
			
			_framesCounter++;
			
			// Should the next texture be displayed ?
			if (_framesCounter >= _framesPerTexture)
			{
				// Has the last texture been displayed ?
				if (_currentTextureId == _finalTextureId)
				{
					LOG_DEBUG("Last texture displayed, removing animated texture.");
					return 1;
				}
				
				// Set next texture to display
				currentTextureId = (int) _currentTextureId;
				currentTextureId++; // Incrementation or addition operators are not defined for TextureManager::TextureId type TODO => cleaner ?
				_currentTextureId = (TextureManager::TextureId) currentTextureId;
				_pointerTexture = TextureManager::getTextureFromId(_currentTextureId);
				
				_framesCounter = 0;
			}
			
			return 0;
		}
};

#endif
