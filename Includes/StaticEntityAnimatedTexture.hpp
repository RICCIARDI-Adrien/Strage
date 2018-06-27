/** @file StaticEntityAnimatedTexture.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_STATIC_ENTITY_ANIMATED_TEXTURE_HPP
#define HPP_STATIC_ENTITY_ANIMATED_TEXTURE_HPP

#include <AnimatedTexture.hpp>
#include <Log.hpp>
#include <StaticEntity.hpp>
#include <TextureManager.hpp>

/** @class StaticEntityAnimatedTexture
 * Display several textures in sequence and stop.
 */
class StaticEntityAnimatedTexture: public StaticEntity
{
	private:
		/** Tell whether the animation finished playing. */
		int _isAnimationTerminated;
	
	public:
		/** Spawn an animated texture at the requested map location.
		 * @param x X coordinate on the map.
		 * @param y Y coordinate on the map.
		 * @param pointerTexture The texture to use.
		 */
		StaticEntityAnimatedTexture(int x, int y, Texture *pointerTexture): StaticEntity(x, y, pointerTexture)
		{
			_isAnimationTerminated = 0;
		}
		
		/** Free allocated resources. */
		virtual ~StaticEntityAnimatedTexture()
		{
			delete _pointerTexture;
		}
		
		virtual void render()
		{
			// Display the animation current image
			if (_pointerTexture->render(_positionRectangle.x - Renderer::displayX, _positionRectangle.y - Renderer::displayY) != 0) _isAnimationTerminated = 1;
		}
		
		/** Update the _pointerTexture with the right texture when time has come. TODO
		 * @return 0 while the animation must be displayed,
		 * @return 1 when the animation is finished and the object must be destroyed.
		 */
		virtual int update()
		{
			return _isAnimationTerminated;
		}
};

#endif
