/** @file FightingEntityEnemyMedium.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_FIGHTING_ENTITY_ENEMY_MEDIUM_HPP
#define HPP_FIGHTING_ENTITY_ENEMY_MEDIUM_HPP

#include <EffectManager.hpp>
#include <EnemyFightingEntity.hpp>
#include <TextureManager.hpp>

/** @class FightingEntityEnemyMedium
 * A medium enemy moving a little slower than small enemy but shooting bigger caliber ammunition.
 */
class FightingEntityEnemyMedium: public EnemyFightingEntity
{
	protected:
		// No need for documentation because it is the same as parent function
		virtual BulletMovingEntity *_fireBullet(int x, int y)
		{
			return new BulletMovingEntity(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP, 5, _facingDirection, 3, 0);
		}
		
	public:
		/** Spawn a medium enemy.
		 * @param x Enemy X coordinate.
		 * @param y Enemy Y coordinate.
		 */
		FightingEntityEnemyMedium(int x, int y): EnemyFightingEntity(x, y, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_FACING_UP, 2, 3, 2000, TextureManager::TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP, EffectManager::EFFECT_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP, EffectManager::EFFECT_ID_MEDIUM_ENEMY_EXPLOSION) {}
		
		/** Free allocated resources. */
		virtual ~FightingEntityEnemyMedium() {}
		
		// No need for documentation because it is the same as parent function
		virtual int update()
		{
			// Update textures to show enemy damage state
			if (_lifePointsAmount == 2)
			{
				_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_UP);
				_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_DOWN);
				_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_LEFT);
				_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_RIGHT);
			}
			else if (_lifePointsAmount == 1)
			{
				_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_UP);
				_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_DOWN);
				_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_LEFT);
				_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_RIGHT);
			}
			
			return EnemyFightingEntity::update();
		}
};

#endif
