#include <irrlicht.h>

#include "CameraAnimator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main()
{
	// Create device
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1280, 720), 16, false, false, false, 0);
	if (!device)
		return 1;
	device->setWindowCaption(L"3D Game");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	IMeshSceneNode* cube = smgr->addCubeSceneNode(1, smgr->getRootSceneNode(), 0, vector3df(2.0f, 0.f, 0.f), vector3df(0.0f, 45.f, 0.f));
	cube->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
	ICameraSceneNode* camera = smgr->addCameraSceneNode(smgr->getRootSceneNode(), vector3df(0.0f, 0.f, 0.f), vector3df(2.0f, 0.f, 0.f), 1);
	ISceneNodeAnimator* animator = new CameraAnimator(driver, device->getCursorControl(), 5.0f);
	camera->addAnimator(animator);
	animator->drop();

	int lastFPS = -1;
	u32 then = device->getTimer()->getTime();

	const float MOVEMENT_SPEED = 5.0f;

	while (device->run())
	{
		// Delta time
		const u32 now = device->getTimer()->getTime();
		const f32 dt = (f32)(now - then) / 1000.0f; // Time in seconds
		then = now;

		// Update
		

		// Draw
		driver->beginScene(true, true, SColor(255, 100, 101, 140));

		smgr->drawAll();

		driver->endScene();

		// FPS
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
	device->drop();

	return 0;
}
