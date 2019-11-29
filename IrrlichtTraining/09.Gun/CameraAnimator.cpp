#include "CameraAnimator.h"

CameraAnimator::CameraAnimator(IVideoDriver* driver, ICursorControl* cursorControl, f32 moveSpeed, f64 rotateSpeed, bool _wallCollisions[][32])
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

	for (int i = 0; i < 32; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			wallCollisions[i][j] = _wallCollisions[i][j];
		}
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

	if (event.MouseInput.isLeftPressed())
	{
		if (!leftMouseHasBeenPressed && !leftMouseBufferOn)
		{
			leftMouseIsJustPressed = true;
			leftMouseBufferOn = true;
		}
		else
		{
			leftMouseIsJustPressed = false;
		}
		leftMouseHasBeenPressed = true;
	}
	else
	{
		leftMouseIsJustPressed = false;
		leftMouseHasBeenPressed = false;
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

	// Collision check
	if (checkCollision(camera, &speed, 0.25f, 10.0f)) {
		speed = vector3df(0.0f, 0.0f, 0.0f);
	}

	// Move
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

const bool CameraAnimator::getLeftMouseIsJustPressed()
{
	bool result = false;
	if (leftMouseIsJustPressed)
	{
		result = true;
		leftMouseIsJustPressed = false;
	}
	return result;
}

bool CameraAnimator::isKeyDown(EKEY_CODE keyCode) const
{
	return keyIsDown[keyCode];
}

bool CameraAnimator::checkCollision(const ISceneNode* node, const vector3df* speed, f32 radius, f32 speedCollisionDivider)
{
	vector2df futurePosition = vector2df(node->getPosition().X + speed->X / speedCollisionDivider + 0.5f, node->getPosition().Z + speed->Z / speedCollisionDivider + 0.5f);
	int minX = (int)(futurePosition.X - radius);
	int maxX = (int)(futurePosition.X + radius);
	int minY = (int)(futurePosition.Y - radius);
	int maxY = (int)(futurePosition.Y + radius);

	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (x < 0 || y < 0 || x >= 32 || y >= 32)
			{
				return true;
			}
			if (wallCollisions[x][y])
			{
				return true;
			}
		}
	}
	return false;
}
