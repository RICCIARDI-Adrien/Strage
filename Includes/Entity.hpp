#ifndef HPP_ENTITY_HPP
#define HPP_ENTITY_HPP

#include <Texture.hpp>

/** @class Entity
 * Base class for all game entity.
 */
class Entity
{
	protected:
		/** The texture used to render the entity. */
		Texture *_pointerTexture;
		
		/** The entity left coordinate on the map. */
		int _x;
		/** The entity top coordinate on the map. */
		int _y;
		
	public:
		//Entity() {}
		
		/** Free entity allocated resources. */
		//virtual ~Entity() = 0;
		
		/** Display the entity at its current location on the map. */
		virtual void render() = 0;
		
		/** Update the internal entity logic.
		 * @return 1 if the entity must be kept alive,
		 * @return 0 if the entity must be deleted.
		 */
		virtual int update() = 0;
};

#endif
