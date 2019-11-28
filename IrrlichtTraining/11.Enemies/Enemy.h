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
private:
	u32 currentFrame;
	f32 frameSpeed;

	ICameraSceneNode* target;
	ITexture* textures[7];
};

