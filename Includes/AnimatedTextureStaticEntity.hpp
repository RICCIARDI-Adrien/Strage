/** @file AnimatedTextureStaticEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_ANIMATED_TEXTURE_STATIC_ENTITY_HPP
#define HPP_ANIMATED_TEXTURE_STATIC_ENTITY_HPP

#include <AnimatedTexture.hpp>
#include <StaticEntity.hpp>

/** @class AnimatedTextureStaticEntity
 * Display several textures in sequence then stop.
 */
class AnimatedTextureStaticEntity: public StaticEntity
{
	public:
		/** Spawn an animated texture at the requested map location.
		 * @param x X coordinate on the map.
		 * @param y Y coordinate on the map.
		 * @param pointerTexture The texture to use. This must be an AnimatedTexture dynamically created (for instance using TextureManager::createAnimatedTextureFromId()).
		 */
		AnimatedTextureStaticEntity(int x, int y, AnimatedTexture *pointerTexture);
		
		// No need for documentation because it is the same as parent function
		virtual ~AnimatedTextureStaticEntity();
		
		// No need for documentation because it is the same as parent function
		virtual void render();
		
		/** Tell whether the animation finished playing.
		 * @return 0 while the animation must be displayed,
		 * @return 1 when the animation is finished and the object must be destroyed.
		 */
		virtual int update();

	private:
		/** Tell whether the animation finished playing. */
		int _isAnimationTerminated;
};

#endif
