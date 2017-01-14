#ifndef HPP_ENTITY_ANIMATED_TEXTURE_ENEMY_SPAWNER_EXPLOSION_HPP
#define HPP_ENTITY_ANIMATED_TEXTURE_ENEMY_SPAWNER_EXPLOSION_HPP

#include <EntityAnimatedTexture.hpp>
#include <TextureManager.hpp>

/** @class EntityAnimatedTextureEnemySpawnerExplosion
 * Display a violent explosion with the same dimensions than an enemy spawner.
 * @author Adrien RICCIARDI
 */
class EntityAnimatedTextureEnemySpawnerExplosion: public EntityAnimatedTexture
{
	public:
		/** Spawn an explosion at the provided coordinates.
		 * @param x X coordinate on the map.
		 * @param y Y coordinate on the map.
		 */
		EntityAnimatedTextureEnemySpawnerExplosion(int x, int y): EntityAnimatedTexture(x, y, TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_0, TextureManager::TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION_15, 8) {}
};

#endif