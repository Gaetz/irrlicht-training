#include "CameraAnimator.h"

CameraAnimator::CameraAnimator(IVideoDriver* driver, ICursorControl* cursorControl, f32 moveSpeed)
	:driver(driver), cursorControl(cursorControl), moveSpeed(moveSpeed)
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		keyIsDown[i] = false;
	}
}

bool CameraAnimator::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	return false;
}

void CameraAnimator::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA) { return; }
	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	const f32 dt = (f32)(timeMs - then) / 1000.0f;
	then = timeMs;

	vector3df cameraPosition = camera->getPosition();
	vector3df speed = vector3df();

	if (isKeyDown(KEY_KEY_Z))
		speed.X += moveSpeed;
	else if (isKeyDown(KEY_KEY_S))
		speed.X -= moveSpeed;

	if (isKeyDown(KEY_KEY_Q))
		speed.Z += moveSpeed;
	else if (isKeyDown(KEY_KEY_D))
		speed.Z -= moveSpeed;

	speed.normalize();
	cameraPosition += speed * dt;
	camera->setPosition(cameraPosition);
	camera->updateAbsolutePosition();

	vector3df cameraTarget = cameraPosition + vector3df(2, 0, 0);
	camera->setTarget(cameraTarget);
}

ISceneNodeAnimator* CameraAnimator::createClone(ISceneNode* node, ISceneManager* newManager)
{
	return nullptr;
}

bool CameraAnimator::isEventReceiverEnabled()const
{
	return true;
}

bool CameraAnimator::isKeyDown(EKEY_CODE keyCode) const
{
	return keyIsDown[keyCode];
}