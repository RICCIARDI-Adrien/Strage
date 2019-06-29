/** @file FightingEntity.cpp
 * See FightingEntity.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <FightingEntity.hpp>
#include <LevelManager.hpp>

FightingEntity::FightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, TextureManager::TextureId facingUpBulletTextureId, EffectManager::EffectId firingEffectId): MovingEntity(x, y, textureId, movingPixelsAmount)
{
	_lifePointsAmount = maximumLifePointsAmount;
	_maximumLifePointsAmount = maximumLifePointsAmount;
	_firingEffectId = firingEffectId;
	
	// Player and enemies collide with walls and enemy spawners
	_collisionBlockContent = LevelManager::BLOCK_CONTENT_WALL | LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
	
	// Cache the offset to add to entity coordinates to make fired bullets start from where the cannon is (bullets are spawned a little nearer from the entity center than the cannon muzzle, so an underneath entity can be hit)
	// Warning : for the underneath entity to be killed, bullet speed must be less than the bullet texture's larger dimension
	Texture *pointerBulletTexture = TextureManager::getTextureFromId(facingUpBulletTextureId); // Use textures to avoid instantiate a bullet to get its dimensions
	_computeBulletStartingPositionOffsets(_pointerTextures[DIRECTION_UP], pointerBulletTexture, _bulletStartingPositionOffsets);
	
	// Cache the offset to add to entity coordinates to put the muzzle flash in front of the cannon
	_computeFiringEffectStartingPositionOffsets(_pointerTextures[DIRECTION_UP], firingEffectId, _firingEffectStartingPositionOffsets);
	
	_timeBetweenShots = timeBetweenShots;
	_lastShotTime = 0; // Allow to shoot immediately
}

FightingEntity::~FightingEntity() {}

bool FightingEntity::modifyLife(int lifePointsAmount)
{
	_lifePointsAmount += lifePointsAmount;
	
	// Clamp life points to [0; _maximumLifePointsAmount]
	if (_lifePointsAmount < 0) _lifePointsAmount = 0;
	else if (_lifePointsAmount > _maximumLifePointsAmount) _lifePointsAmount = _maximumLifePointsAmount;
	
	return true;
}

BulletMovingEntity *FightingEntity::shoot()
{
	// Allow to shoot only if enough time elapsed since last shot
	if (SDL_GetTicks() - _lastShotTime >= _timeBetweenShots)
	{
		// Cache entity coordinates
		int entityX = _positionRectangles[_facingDirection].x;
		int entityY = _positionRectangles[_facingDirection].y;
		
		// Select the right offsets according to entity direction
		int bulletStartingPositionOffsetX = _bulletStartingPositionOffsets[_facingDirection].x;
		int bulletStartingPositionOffsetY = _bulletStartingPositionOffsets[_facingDirection].y;
		int firingEffectStartingPositionOffsetX = _firingEffectStartingPositionOffsets[_facingDirection].x;
		int firingEffectStartingPositionOffsetY = _firingEffectStartingPositionOffsets[_facingDirection].y;
		
		// Create the bullet
		BulletMovingEntity *pointerBullet = _fireBullet(entityX + bulletStartingPositionOffsetX, entityY + bulletStartingPositionOffsetY);
		
		// Play the shoot effect
		EffectManager::EffectId muzzleFlashEffectId = (EffectManager::EffectId) ((int) _firingEffectId + (int) _facingDirection); // Select the right effect according to entity direction
		EffectManager::addEffect(entityX + firingEffectStartingPositionOffsetX, entityY + firingEffectStartingPositionOffsetY, muzzleFlashEffectId);
		
		// Get time after having generated the bullet, in case this takes more than 1 millisecond
		_lastShotTime = SDL_GetTicks();
		
		return pointerBullet;
	}
	return NULL; // No shot allowed
}

void FightingEntity::_computeBulletStartingPositionOffsets(Texture *pointerEntityTexture, Texture *pointerFacingUpBulletTexture, SDL_Point *pointerPositionOffsets)
{
	// Cache dimensions
	int entityWidth = pointerEntityTexture->getWidth(); // Only entity width is required because the entity is always facing the direction it shoots to
	int bulletWidth = pointerFacingUpBulletTexture->getWidth();
	int bulletHeight = pointerFacingUpBulletTexture->getHeight();
	
	// Assume that entity is faced to the direction the bullet is fired
	pointerPositionOffsets[DIRECTION_UP].x = (entityWidth - bulletWidth) / 2;
	pointerPositionOffsets[DIRECTION_UP].y = 2; // Make the bullet start into the entity, with two more pixels to be sure to hit an underneath entity
	pointerPositionOffsets[DIRECTION_DOWN].x = pointerPositionOffsets[DIRECTION_UP].x;
	pointerPositionOffsets[DIRECTION_DOWN].y = entityWidth - bulletHeight - 2; // -1 should be enough due entityHeight usage (which results in coordinate + 1), but -2 is needed to make the underneath entity killable
	pointerPositionOffsets[DIRECTION_LEFT].x = 2; // Manually adjusted value to allow an underneath entity to be hit when this entity is facing left (TODO dependent of bullet size and speed)
	pointerPositionOffsets[DIRECTION_LEFT].y = pointerPositionOffsets[DIRECTION_UP].x;
	pointerPositionOffsets[DIRECTION_RIGHT].x = pointerPositionOffsets[DIRECTION_DOWN].y; // Entity is facing right, so its horizontal width is its height
	pointerPositionOffsets[DIRECTION_RIGHT].y = pointerPositionOffsets[DIRECTION_UP].x;
}

void FightingEntity::_computeFiringEffectStartingPositionOffsets(Texture *pointerEntityTexture, EffectManager::EffectId firingEffectId, SDL_Point *pointerPositionOffsets)
{
	// Cache dimensions
	int entityWidth = pointerEntityTexture->getWidth();
	int entityHeight = pointerEntityTexture->getHeight();
	int firingEffectWidth = EffectManager::getEffectTextureWidth(firingEffectId);
	int firingEffectHeight = EffectManager::getEffectTextureHeight(firingEffectId);
	
	// Assume that entity is faced to the direction the fire effect is spawned
	pointerPositionOffsets[DIRECTION_UP].x = (entityWidth - firingEffectWidth) / 2;
	pointerPositionOffsets[DIRECTION_UP].y = -firingEffectHeight;
	pointerPositionOffsets[DIRECTION_DOWN].x = pointerPositionOffsets[DIRECTION_UP].x;
	pointerPositionOffsets[DIRECTION_DOWN].y = entityHeight;
	pointerPositionOffsets[DIRECTION_LEFT].x = -firingEffectHeight;
	pointerPositionOffsets[DIRECTION_LEFT].y = pointerPositionOffsets[DIRECTION_UP].x;
	pointerPositionOffsets[DIRECTION_RIGHT].x = pointerPositionOffsets[DIRECTION_DOWN].y;
	pointerPositionOffsets[DIRECTION_RIGHT].y = pointerPositionOffsets[DIRECTION_UP].x;
}
