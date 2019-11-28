#pragma once
#include "Entity.h"
class Enemy :
	public Entity
{
public:
	Enemy();
	virtual void update(const f32 dt);
	void setTarget(ICameraSceneNode*);
	void setTextures(ITexture* textures[7]);
	virtual void takeDamage(u32 damages);

private:
	u32 currentFrame;
	f32 frameSpeed;
	u32 lives;

	ICameraSceneNode* target;
	ITexture* textures[7];
};

