#include "Enemy.h"

Enemy::Enemy()
	: Entity(), currentFrame(0), frameSpeed(0.15f)
{
}

void Enemy::update(const f32 dt)
{
	frameSpeed -= dt;
	if(frameSpeed <= 0) 
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
