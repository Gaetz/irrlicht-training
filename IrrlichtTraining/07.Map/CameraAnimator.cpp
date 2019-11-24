#include "CameraAnimator.h"

CameraAnimator::CameraAnimator(IVideoDriver* driver, ICursorControl* cursorControl, f32 moveSpeed, f64 rotateSpeed)
	:driver(driver), cursorControl(cursorControl), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed), firstFrame(true)
{
	cursorOrigin = vector2df(0.5f, 0.5f);
	cursorPos = cursorOrigin;
	cursorControl->setPosition(cursorOrigin);
	cursorMove = position2d<f32>(0, 0);

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

	if (event.MouseInput.Event == EMIE_MOUSE_MOVED) 
	{
		cursorMove = cursorControl->getRelativePosition() - cursorPos;
		cursorControl->setPosition(0.5f, 0.5f);
		cursorPos = cursorOrigin;
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
	vector3df cameraRotation = camera->getRotation();

	// Rotation change
	f32 rotationY = 0.f;
	f32 rotationZ = 0.f;
	if (cursorMove != vector2df(0, 0))
	{
		rotationY = cursorMove.X * rotateSpeed * 2;
		rotationZ = cursorMove.Y * rotateSpeed;
	}
	vector3df deltaRotation = vector3df(0, rotationY, rotationZ);

	quaternion qCamera = quaternion(cameraRotation * DEGTORAD);
	quaternion qDelta = quaternion(deltaRotation * DEGTORAD);
	quaternion qFinaleRotation = qCamera * qDelta;

	vector3df gimbalFreeAngle;
	qFinaleRotation.toEuler(gimbalFreeAngle);
	gimbalFreeAngle *= RADTODEG;
	camera->setRotation(gimbalFreeAngle);

	forward = camera->getTarget() - cameraPosition;
	forward.normalize();

	// Position change
	vector3df speed = vector3df();

	if (isKeyDown(KEY_KEY_Z))
		speed += forward;
	else if (isKeyDown(KEY_KEY_S))
		speed -= forward;

	vector3df right = forward;
	right.rotateXZBy(90);

	if (isKeyDown(KEY_KEY_Q))
		speed += right;
	else if (isKeyDown(KEY_KEY_D))
		speed -= right;

	speed.normalize();
	speed.Y = 0;
	cameraPosition += speed * moveSpeed * dt;
	camera->setPosition(cameraPosition);
	camera->updateAbsolutePosition();

	cursorMove = vector2df(0, 0);
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