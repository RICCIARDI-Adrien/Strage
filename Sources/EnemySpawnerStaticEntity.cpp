/** @file EnemySpawnerStaticEntity.cpp
 * See EnemySpawnerStaticEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <EffectManager.hpp>
#include <EnemySpawnerStaticEntity.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

EnemySpawnerStaticEntity::EnemySpawnerStaticEntity(int x, int y): StaticEntity(x, y, TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER))
{
	_lifePointsAmount = 10;
	
	// Create the looped teleportation animation
	_pointerEffectTexture = TextureManager::createAnimatedTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_TELEPORTATION_EFFECT, true);
}

EnemySpawnerStaticEntity::~EnemySpawnerStaticEntity()
{
	delete _pointerEffectTexture;
}

void EnemySpawnerStaticEntity::render()
{
	// Display spawner
	StaticEntity::render();
	
	// Display teleportation effect
	_pointerEffectTexture->render(_positionRectangle.x - Renderer::displayX + 15, _positionRectangle.y - Renderer::displayY + 15);
}

int EnemySpawnerStaticEntity::update()
{
	// Remove the spawner if it is destroyed
	if (_lifePointsAmount == 0)
	{
		// Display an explosion
		int soundEmitterAngle = 0, soundEmitterDistance = 0;
		AudioManager::computePositionFromCamera(_positionRectangle.x + _pointerTexture->getWidth() / 2, _positionRectangle.y + _pointerTexture->getHeight() / 2, &soundEmitterAngle, &soundEmitterDistance);
		EffectManager::addEffect(_positionRectangle.x, _positionRectangle.y, EffectManager::EFFECT_ID_ENEMY_SPAWNER_EXPLOSION, soundEmitterAngle, soundEmitterDistance);
		
		return 1;
	}
	
	// Update textures to show spawner damage state
	if (_lifePointsAmount == 8) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_1);
	else if (_lifePointsAmount == 6) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_2);
	else if (_lifePointsAmount == 4) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_3);
	else if (_lifePointsAmount == 2) _pointerTexture = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_4);
	
	return 0;
}
