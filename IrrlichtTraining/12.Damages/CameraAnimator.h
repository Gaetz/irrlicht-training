#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::gui;
using namespace irr::core;

class CameraAnimator : public ISceneNodeAnimator
{
public:
	CameraAnimator(IVideoDriver* driver, ICursorControl* cursorControl, f32 moveSpeed, f64 rotateSpeed, bool _wallCollisions[][32]);

	virtual bool OnEvent(const SEvent& event);
	virtual void animateNode(ISceneNode* node, u32 timeMs);
	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
	virtual bool isEventReceiverEnabled() const;

	const bool getLeftMouseIsJustPressed();

private:
	// This is used to check whether a key is being held down
	virtual bool isKeyDown(EKEY_CODE keyCode) const;

	bool checkCollision(const ISceneNode* node, const vector3df* speed, f32 radius, f32 speedCollisionDivider);

	// We use this array to store the current state of each key
	bool keyIsDown[KEY_KEY_CODES_COUNT];
	bool leftMouseIsJustPressed;
	bool leftMouseHasBeenPressed;

	IVideoDriver* driver;
	ICursorControl* cursorControl;
	f32 moveSpeed;
	u32 then;

	position2d<f32> cursorMove;
	position2d<f32> cursorPos;
	position2d<f32> cursorOrigin;
	f64 rotateSpeed;
	vector3df forward;

	bool firstFrame;
	bool wallCollisions[32][32];
};

