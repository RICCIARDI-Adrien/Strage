/** @file PlayerFightingEntity.cpp
 * See PlayerFightingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <Configuration.hpp>
#include <EffectManager.hpp>
#include <HeadUpDisplay.hpp>
#include <LevelManager.hpp>
#include <Log.hpp>
#include <PlayerFightingEntity.hpp>
#include <Renderer.hpp>
#include <TextureManager.hpp>

PlayerFightingEntity::PlayerFightingEntity(int x, int y): FightingEntity(x, y, TextureManager::TEXTURE_ID_PLAYER_FACING_UP, 3, CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_LIFE_POINTS_AMOUNT, 300, TextureManager::TEXTURE_ID_PLAYER_BULLET_FACING_UP, EffectManager::EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_UP)
{
	
	// Cache rendering coordinates
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	_renderingX = (Renderer::displayWidth - pointerPositionRectangle->w) / 2;
	_renderingY = (Renderer::displayHeight - pointerPositionRectangle->h) / 2;
	
	_ammunitionAmount = CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_AMMUNITION_AMOUNT;
	
	// Allow to shoot immediately
	_secondaryFireTimeBetweenShots = 9500; // Slightly faster than a big enemy reloading time
	_secondaryFireLastShotTime = -_secondaryFireTimeBetweenShots;
	
	// Cache bullet and firing effect position offsets
	_computeBulletStartingPositionOffsets(_pointerTextures[DIRECTION_UP], TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP), _secondaryFireStartingPositionOffsets);
	// Secondary firing effect position
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_UP].x = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_UP_STARTING_POSITION_OFFSET_X;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_UP].y = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_UP_STARTING_POSITION_OFFSET_Y;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_DOWN].x = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_DOWN_STARTING_POSITION_OFFSET_X;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_DOWN].y = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_DOWN_STARTING_POSITION_OFFSET_Y;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_LEFT].x = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_LEFT_STARTING_POSITION_OFFSET_X;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_LEFT].y = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_LEFT_STARTING_POSITION_OFFSET_Y;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_RIGHT].x = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_RIGHT_STARTING_POSITION_OFFSET_X;
	_secondaryFireFiringEffectStartingPositionOffsets[DIRECTION_RIGHT].y = CONFIGURATION_PLAYER_MUZZLE_FLASH_MORTAR_SHELL_FACING_RIGHT_STARTING_POSITION_OFFSET_Y;
}

PlayerFightingEntity::~PlayerFightingEntity() {}

BulletMovingEntity *PlayerFightingEntity::shoot()
{
	// The player can't shoot if it has no more ammunition
	if (_ammunitionAmount == 0) return NULL;
	
	// Decrement ammunition count if the player shot
	BulletMovingEntity *pointerBullet = FightingEntity::shoot();
	if (pointerBullet != NULL)
	{
		_ammunitionAmount--;
		return pointerBullet;
	}
	return NULL;
}

BulletMovingEntity *PlayerFightingEntity::shootSecondaryFire()
{
	// The player can't shoot if it has no more ammunition
	if (_ammunitionAmount < CONFIGURATION_GAMEPLAY_PLAYER_SECONDARY_FIRE_NEEDED_AMMUNITION_AMOUNT) return NULL;
	
	// Allow to shoot only if enough time elapsed since last shot
	if (_isSecondaryShootReloadingTimeElapsed)
	{
		// Cache entity coordinates
		int entityX = _positionRectangles[_facingDirection].x;
		int entityY = _positionRectangles[_facingDirection].y;
		
		// Select the right offsets according to entity direction
		int bulletStartingPositionOffsetX = _secondaryFireStartingPositionOffsets[_facingDirection].x;
		int bulletStartingPositionOffsetY = _secondaryFireStartingPositionOffsets[_facingDirection].y;
		int firingEffectStartingPositionOffsetX = _secondaryFireFiringEffectStartingPositionOffsets[_facingDirection].x;
		int firingEffectStartingPositionOffsetY = _secondaryFireFiringEffectStartingPositionOffsets[_facingDirection].y;
		
		// Create the bullet
		BulletMovingEntity *pointerBullet = new BulletMovingEntity(entityX + bulletStartingPositionOffsetX, entityY + bulletStartingPositionOffsetY, TextureManager::TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP, 4, _facingDirection, 20, 1);
		
		_ammunitionAmount -= CONFIGURATION_GAMEPLAY_PLAYER_SECONDARY_FIRE_NEEDED_AMMUNITION_AMOUNT;
		
		// Play the shoot effect
		EffectManager::addEffect(entityX + firingEffectStartingPositionOffsetX, entityY + firingEffectStartingPositionOffsetY, EffectManager::EFFECT_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL);
		// There are enough ammunition to also play the reloading effect (it can be started at the same time that the firing sound because there is a purposely added delay in reloading sound)
		if (_ammunitionAmount >= CONFIGURATION_GAMEPLAY_PLAYER_SECONDARY_FIRE_NEEDED_AMMUNITION_AMOUNT) AudioManager::playSound(AudioManager::SOUND_ID_PLAYER_MORTAR_SHELL_RELOADING);
		
		// Get time after having generated the bullet, in case this takes more than 1 millisecond
		_isSecondaryShootReloadingTimeElapsed = false;
		_secondaryFireLastShotTime = SDL_GetTicks();
		
		return pointerBullet;
	}
	
	return NULL; // No shot allowed
}

void PlayerFightingEntity::render()
{
	// Display the player at the screen center
	_pointerTextures[_facingDirection]->render(_renderingX, _renderingY);
}

int PlayerFightingEntity::update()
{
	// Check whether the mortar reloading time has elapsed
	if (SDL_GetTicks() - _secondaryFireLastShotTime >= _secondaryFireTimeBetweenShots) _isSecondaryShootReloadingTimeElapsed = true;
	
	// Update mortar HUD state
	if (_ammunitionAmount < CONFIGURATION_GAMEPLAY_PLAYER_SECONDARY_FIRE_NEEDED_AMMUNITION_AMOUNT) HeadUpDisplay::setMortarState(HeadUpDisplay::MORTAR_STATE_LOW_AMMUNITION);
	else
	{
		if (!_isSecondaryShootReloadingTimeElapsed) HeadUpDisplay::setMortarState(HeadUpDisplay::MORTAR_STATE_RELOADING);
		else HeadUpDisplay::setMortarState(HeadUpDisplay::MORTAR_STATE_READY);
	}
	
	// Cache player center coordinates
	SDL_Rect *pointerPositionRectangle = &_positionRectangles[_facingDirection];
	int playerCenterX = pointerPositionRectangle->x + (pointerPositionRectangle->w / 2);
	int playerCenterY = pointerPositionRectangle->y + (pointerPositionRectangle->h / 2);
	
	// Cache the coordinates of the block the player is crossing
	int blockX = playerCenterX - (playerCenterX % CONFIGURATION_LEVEL_BLOCK_SIZE);
	int blockY = playerCenterY - (playerCenterY % CONFIGURATION_LEVEL_BLOCK_SIZE);
	
	// Get block under player center content
	int blockContent = LevelManager::getBlockContent(playerCenterX, playerCenterY);
	
	// Is there a medipack ?
	if (blockContent & LevelManager::BLOCK_CONTENT_MEDIPACK)
	{
		LOG_DEBUG("Player is crossing a block containing a medipack.");
		
		if (isWounded())
		{
			modifyLife(20);
			EffectManager::addEffect(blockX, blockY, EffectManager::EFFECT_ID_MEDIPACK_TAKEN);
			LOG_DEBUG("Healed player.");
			
			// Remove the medipack as it has been used
			blockContent &= ~LevelManager::BLOCK_CONTENT_MEDIPACK;
			LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
		}
	}
	// Is there a golden medipack ?
	if (blockContent & LevelManager::BLOCK_CONTENT_GOLDEN_MEDIPACK)
	{
		LOG_DEBUG("Player is crossing a block containing a golden medipack.");
		
		// Increase maximum life by 20%
		_maximumLifePointsAmount = (_maximumLifePointsAmount * 120) / 100;
		// Restore life
		_lifePointsAmount = _maximumLifePointsAmount;
		EffectManager::addEffect(blockX, blockY, EffectManager::EFFECT_ID_GOLDEN_MEDIPACK_TAKEN);
		LOG_DEBUG("Increased player life and healed him.");
		
		// Remove the golden medipack as it has been used
		blockContent &= ~LevelManager::BLOCK_CONTENT_GOLDEN_MEDIPACK;
		LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
	}
	// Is there ammunition ?
	else if (blockContent & LevelManager::BLOCK_CONTENT_AMMUNITION)
	{
		LOG_DEBUG("Player is crossing a block containing ammunition.");
		
		addAmmunition(30);
		EffectManager::addEffect(blockX, blockY, EffectManager::EFFECT_ID_AMMUNITION_TAKEN);
		LOG_DEBUG("Player got ammunition.");
		
		// Remove the ammunition item
		blockContent &= ~LevelManager::BLOCK_CONTENT_AMMUNITION;
		LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
	}
	// Is it the level end ?
	else if (blockContent & LevelManager::BLOCK_CONTENT_LEVEL_EXIT) return 2;
	
	return 0;
}

BulletMovingEntity *PlayerFightingEntity::_fireBullet(int x, int y)
{
	return new BulletMovingEntity(x, y, TextureManager::TEXTURE_ID_PLAYER_BULLET_FACING_UP, 6, _facingDirection, 1, 1);
}
