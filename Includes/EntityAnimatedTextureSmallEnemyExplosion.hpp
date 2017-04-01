#ifndef HPP_ENTITY_ANIMATED_TEXTURE_SMALL_ENEMY_EXPLOSION_HPP
#define HPP_ENTITY_ANIMATED_TEXTURE_SMALL_ENEMY_EXPLOSION_HPP

#include <EntityAnimatedTexture.hpp>
#include <TextureManager.hpp>

/** @class EntityAnimatedTextureSmallEnemyExplosion
 * Display a violent explosion with the same dimensions than a small enemy.
 * @author Adrien RICCIARDI
 */
class EntityAnimatedTextureSmallEnemyExplosion: public EntityAnimatedTexture
{
	public:
		/** Spawn an explosion at the provided coordinates.
		 * @param x X coordinate on the map.
		 * @param y Y coordinate on the map.
		 */
		EntityAnimatedTextureSmallEnemyExplosion(int x, int y): EntityAnimatedTexture(x, y, TextureManager::TEXTURE_ID_SMALL_ENEMY_EXPLOSION_0, TextureManager::TEXTURE_ID_SMALL_ENEMY_EXPLOSION_15, 8) {}
};

#endif