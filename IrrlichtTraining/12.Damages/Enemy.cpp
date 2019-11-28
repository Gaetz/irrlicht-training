#include "Enemy.h"

Enemy::Enemy()
	: Entity(), currentFrame(0), frameSpeed(0.15f), lives(10)
{
}

void Enemy::update(const f32 dt)
{
	if (lives <= 0)
	{
		if (currentFrame < 4)
		{
			currentFrame = 4;
		}
		frameSpeed -= dt;
		if (frameSpeed <= 0)
		{
			frameSpeed += 0.15f;
			++currentFrame;
			if (currentFrame > 6)
			{
				currentFrame = 6;
			}
			sprite->setMaterialTexture(0, textures[currentFrame]);
		}
	}
	else
	{
		frameSpeed -= dt;
		if (frameSpeed <= 0)
		{
			frameSpeed += 0.15f;
			++currentFrame;
			if (currentFrame > 1)
			{
				currentFrame = 0;
			}
			sprite->setMaterialTexture(0, textures[currentFrame]);
		}
	}
}

void Enemy::setTarget(ICameraSceneNode* target)
{
	this->target = target;
}

void Enemy::setTextures(ITexture* textures[7])
{
	for (u32 i = 0; i < 7; ++i)
	{
		this->textures[i] = textures[i];
	}
}

void Enemy::takeDamage(u32 damages)
{
	lives -= damages;
}
