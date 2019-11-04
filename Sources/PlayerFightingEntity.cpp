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

PlayerFightingEntity::PlayerFightingEntity(int x, int y): FightingEntity(x, y, TextureManager::TEXTURE_ID_PLAYER_FACING_UP, 3, CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_LIFE_POINTS_AMOUNT, CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_PRIMARY_FIRE_TIME_BETWEEN_TWO_SHOTS, TextureManager::TEXTURE_ID_PLAYER_BULLET_FACING_UP, EffectManager::EFFECT_ID_PLAYER_MUZZLE_FLASH_FACING_UP)
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

bool PlayerFightingEntity::modifyLife(int lifePointsAmount)
{
	// Do not wound player if he is wearing a bulletproof vest, but permit life restoration
	if ((_currentActiveBonus == BONUS_BULLETPROOF_VEST) && (lifePointsAmount < 0)) return false;
	
	// No defensive bonus or life restoration
	FightingEntity::modifyLife(lifePointsAmount);
	return true;
}

BulletMovingEntity *PlayerFightingEntity::shoot()
{
	// The player can't shoot if it has no more ammunition
	if ((_currentActiveBonus != BONUS_MACHINE_GUN) && (_ammunitionAmount == 0)) return NULL; // Ammunition are unlimited with "machine gun" bonus
	
	// Decrement ammunition count if the player shot
	BulletMovingEntity *pointerBullet = FightingEntity::shoot();
	if (pointerBullet != NULL)
	{
		if (_currentActiveBonus != BONUS_MACHINE_GUN) _ammunitionAmount--; // Ammunition are unlimited with "machine gun" bonus
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
	
	// Handle bonus
	if (_bonusRemainingTime > 0)
	{
		HeadUpDisplay::setRemainingBonusTime(((_bonusRemainingTime * CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) / 1000) + 1); // Convert frame time to seconds, add one more second to compensate for integer computation rounding
		_bonusRemainingTime--;
	}
	else
	{
		// Disable bonus time displaying
		HeadUpDisplay::setRemainingBonusTime(0);
		
		// Restore player default attributes
		_timeBetweenShots = CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_PRIMARY_FIRE_TIME_BETWEEN_TWO_SHOTS;
		_currentActiveBonus = BONUS_NONE;
		
		// Restore default player skin
		_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_UP);
		_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_DOWN);
		_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_LEFT);
		_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_FACING_RIGHT);
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
	// Is there a machine gun bonus ?
	else if (blockContent & LevelManager::BLOCK_CONTENT_MACHINE_GUN_BONUS)
	{
		LOG_DEBUG("Player is crossing a block containing machine gun bonus.");
		
		// Bonus can't be taken if another bonus is currently in use
		if (_bonusRemainingTime == 0)
		{
			// Configure bonus
			_bonusRemainingTime = 30000 / CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS; // Initialize timer, bonus effect lasts 30s (this method is called each game frame, so adjust time)
			_timeBetweenShots = CONFIGURATION_GAMEPLAY_PLAYER_DEFAULT_PRIMARY_FIRE_TIME_BETWEEN_TWO_SHOTS / 3; // Make shoots three times faster
			_currentActiveBonus = BONUS_MACHINE_GUN;
			
			EffectManager::addEffect(blockX, blockY, EffectManager::EFFECT_ID_MACHINE_GUN_TAKEN);
			LOG_DEBUG("Player got machine gun bonus.");
			
			// Set specific player skin
			_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_UP);
			_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_DOWN);
			_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_LEFT);
			_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_MACHINE_GUN_BONUS_FACING_RIGHT);
			
			// Remove the bonus item
			blockContent &= ~LevelManager::BLOCK_CONTENT_MACHINE_GUN_BONUS;
			LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
		}
	}
	// Is there a bulletproof vest bonus ?
	else if (blockContent & LevelManager::BLOCK_CONTENT_BULLETPROOF_VEST_BONUS)
	{
		LOG_DEBUG("Player is crossing a block containing bulletproof vest bonus.");
		
		// Bonus can't be taken if another bonus is currently in use
		if (_bonusRemainingTime == 0)
		{
			// Configure bonus
			_bonusRemainingTime = 45000 / CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS; // Initialize timer, bonus effect lasts 45s (this method is called each game frame, so adjust time)
			_currentActiveBonus = BONUS_BULLETPROOF_VEST;
			
			EffectManager::addEffect(blockX, blockY, EffectManager::EFFECT_ID_BULLETPROOF_VEST_TAKEN);
			LOG_DEBUG("Player got bulletproof vest bonus.");
			
			// Set specific player skin
			_pointerTextures[DIRECTION_UP] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_UP);
			_pointerTextures[DIRECTION_DOWN] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_DOWN);
			_pointerTextures[DIRECTION_LEFT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_LEFT);
			_pointerTextures[DIRECTION_RIGHT] = TextureManager::getTextureFromId(TextureManager::TEXTURE_ID_PLAYER_BULLETPROOF_VEST_BONUS_FACING_RIGHT);
			
			// Remove the bonus item
			blockContent &= ~LevelManager::BLOCK_CONTENT_BULLETPROOF_VEST_BONUS;
			LevelManager::setBlockContent(playerCenterX, playerCenterY, blockContent);
		}
	}
	// Is it the level end ?
	else if (blockContent & LevelManager::BLOCK_CONTENT_LEVEL_EXIT) return 2;
	
	return 0;
}

BulletMovingEntity *PlayerFightingEntity::_fireBullet(int x, int y)
{
	return new BulletMovingEntity(x, y, TextureManager::TEXTURE_ID_PLAYER_BULLET_FACING_UP, 6, _facingDirection, 1, 1);
}
