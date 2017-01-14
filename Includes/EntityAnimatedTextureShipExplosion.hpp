#ifndef HPP_ENTITY_ANIMATED_TEXTURE_SHIP_EXPLOSION_HPP
#define HPP_ENTITY_ANIMATED_TEXTURE_SHIP_EXPLOSION_HPP

#include <EntityAnimatedTexture.hpp>
#include <TextureManager.hpp>

/** @class EntityAnimatedTextureShipExplosion
 * Display a violent explosion with the same dimensions than an enemy ship (or the player one).
 * @author Adrien RICCIARDI
 */
class EntityAnimatedTextureShipExplosion: public EntityAnimatedTexture
{
	public:
		/** Spawn an explosion at the provided coordinates.
		 * @param x X coordinate on the map.
		 * @param y Y coordinate on the map.
		 */
		EntityAnimatedTextureShipExplosion(int x, int y): EntityAnimatedTexture(x, y, TextureManager::TEXTURE_ID_SHIP_EXPLOSION_0, TextureManager::TEXTURE_ID_SHIP_EXPLOSION_15, 8) {}
};

#endif