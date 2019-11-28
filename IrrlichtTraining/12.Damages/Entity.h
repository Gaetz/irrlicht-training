#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Entity
{
public:
	Entity();
	~Entity();
	void create(ISceneManager* smgr, u32 x, u32 y);

	const bool getDestroyed() const;
	const vector2df getPosition() const;

	virtual void update(const f32 dt);
	virtual void takeDamage(u32 damages);
	void destroy();

protected:
	IBillboardSceneNode* sprite;
	ISceneManager* smgr;
	bool isDestroyed;
};

