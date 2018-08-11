/** @file StaticEntityEnemySpawner.cpp
 * See StaticEntityEnemySpawner.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <StaticEntityEnemySpawner.hpp>
#include <TextureManager.hpp>

StaticEntityEnemySpawner::StaticEntityEnemySpawner(int x, int y): StaticEntity(x, y, TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER))
{
	_lifePointsAmount = 10;
}

int StaticEntityEnemySpawner::update()
{
	// Remove the spawner if it is destroyed
	if (_lifePointsAmount == 0) return 1;
	
	// Update textures to show spawner damage state
	if (_lifePointsAmount == 8) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_1);
	else if (_lifePointsAmount == 6) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_2);
	else if (_lifePointsAmount == 4) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_3);
	else if (_lifePointsAmount == 2) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_4);
	
	return 0;
}
