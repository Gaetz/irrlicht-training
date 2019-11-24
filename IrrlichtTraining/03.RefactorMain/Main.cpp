#include <irrlicht.h>

#include "CameraAnimator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

f32 computeDeltaTime();
void update(const f32 dt);
void draw();
void computeFps();

IrrlichtDevice* device = nullptr;
IVideoDriver* driver = nullptr;
ISceneManager* smgr = nullptr;
IGUIEnvironment* guienv = nullptr;

IMeshSceneNode* cube = nullptr;
ICameraSceneNode* camera = nullptr;

int lastFPS = -1;
u32 then;

const float MOVEMENT_SPEED = 5.0f;

int main()
{
	// Init game
	device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1280, 720), 16, false, false, false, 0);
	if (!device)
		return 1;
	device->setWindowCaption(L"3D Game");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	cube = smgr->addCubeSceneNode(1, smgr->getRootSceneNode(), 0, vector3df(2.0f, 0.f, 0.f), vector3df(0.0f, 45.f, 0.f));
	cube->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
	camera = smgr->addCameraSceneNode(smgr->getRootSceneNode(), vector3df(0.0f, 0.f, 0.f), vector3df(2.0f, 0.f, 0.f), 1);
	ISceneNodeAnimator* animator = new CameraAnimator(driver, device->getCursorControl(), 5.0f);
	camera->addAnimator(animator);
	animator->drop();

	lastFPS = -1;
	then = device->getTimer()->getTime();

	// Main loop
	while (device->run())
	{
		f32 dt = computeDeltaTime();
		update(dt);
		draw();
		computeFps();
	}

	// Exit
	device->drop();

	return 0;
}

f32 computeDeltaTime()
{
	const u32 now = device->getTimer()->getTime();
	const f32 deltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	then = now;
	return deltaTime;
}

void update(const f32 dt)
{
	
}

void draw()
{
	driver->beginScene(true, true, SColor(255, 100, 101, 140));

	smgr->drawAll();

	driver->endScene();
}

void computeFps()
{
	int fps = driver->getFPS();
	if (lastFPS != fps)
	{
		core::stringw tmp(L"3D Game [");
		tmp += driver->getName();
		tmp += L"] fps: ";
		tmp += fps;

		device->setWindowCaption(tmp.c_str());
		lastFPS = fps;
	}
}