#include <irrlicht.h>
#include <irrMath.h>

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

ITexture* groundTexture = nullptr;
ITexture* wallTexture = nullptr;
ITexture* ceilTexture = nullptr;

int lastFPS = -1;
u32 then;

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
	ISceneNodeAnimator* animator = new CameraAnimator(driver, device->getCursorControl(), 5.0f, 30.f);
	camera->addAnimator(animator);
	camera->bindTargetAndRotation(true);
	camera->setTarget(cube->getAbsolutePosition());
	animator->drop();
	device->getCursorControl()->setVisible(false);

	lastFPS = -1;
	then = device->getTimer()->getTime();

	// Environment
	groundTexture = driver->getTexture("ground.png");
	wallTexture = driver->getTexture("wall.png");
	ceilTexture = driver->getTexture("ceil.png");
	cube->setMaterialTexture(0, wallTexture);

	IAnimatedMesh* groundMesh = smgr->addHillPlaneMesh("plane", dimension2df(1, 1), dimension2du(32, 32), nullptr, 0, dimension2df(0, 0), dimension2df(32, 32));
	IMeshSceneNode* ground = smgr->addMeshSceneNode(groundMesh);
	ground->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
	ground->setMaterialTexture(0, groundTexture);
	ground->setPosition(vector3df(15.5f, -0.5f, 15.5f));

	IMeshSceneNode* ceiling = smgr->addMeshSceneNode(groundMesh);
	ceiling->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
	ceiling->setMaterialTexture(0, ceilTexture);
	ceiling->setPosition(vector3df(15.5f, 0.5f, 15.5f));
	ceiling->setRotation(vector3df(180.0f, 0.0f, 0.0f));

	camera->setNearValue(0.1);

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