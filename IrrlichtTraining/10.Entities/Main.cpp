#include <irrlicht.h>
#include <irrMath.h>
#include <iostream>
#include <string> 
#include <vector> 

#include "CameraAnimator.h"
#include "Entity.h"

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
unsigned char* readBitmap(const char* filename);

template<class T>
T* addEntity(u32 x, u32 y);

IrrlichtDevice* device = nullptr;
IVideoDriver* driver = nullptr;
ISceneManager* smgr = nullptr;
IGUIEnvironment* guienv = nullptr;

IMeshSceneNode* cube = nullptr;
ICameraSceneNode* camera = nullptr;

ITexture* groundTexture = nullptr;
ITexture* wallTexture = nullptr;
ITexture* wallAltTexture = nullptr;
ITexture* ceilTexture = nullptr;
bool wallCollisions[32][32];

int lastFPS = -1;
u32 then;

ITexture* gunTextures[3];
u32 gunCurrentFrame = 0;
f32 gunNextFrameCounter = 0.0f;

auto entities = std::vector<Entity*>();

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

	lastFPS = -1;
	then = device->getTimer()->getTime();

	// Environment
	groundTexture = driver->getTexture("ground.png");
	wallTexture = driver->getTexture("wall.png");
	ceilTexture = driver->getTexture("ceil.png");
	wallAltTexture = driver->getTexture("wall_decorated.png");
	for (int i = 0; i < 3; i++)
	{
		std::string filename = "gun" + std::to_string(i) + ".png";
		gunTextures[i] = driver->getTexture(filename.c_str());
	}

	// Will create a white sprite
	addEntity<Entity>(3, 3);

	unsigned char* map = readBitmap("map.bmp");
	u32 i = 0;
	for (int x = 0; x < 32; ++x) 
	{
		for (int y = 0; y < 32; ++y)
		{
			wallCollisions[x][y] = false;
			int r = (int)map[i++];
			int g = (int)map[i++];
			int b = (int)map[i++];
			if ((r == 255) && (g == 255) && (b == 255))
			{
				IMeshSceneNode* cube = smgr->addCubeSceneNode(1, smgr->getRootSceneNode(), 0, vector3df(x, 0.f, y));
				cube->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
				cube->setMaterialTexture(0, wallTexture);
				wallCollisions[x][y] = true;
			}
			else if ((r == 0) && (g == 0) && (b == 255))
			{
				IMeshSceneNode* cube = smgr->addCubeSceneNode(1, smgr->getRootSceneNode(), 0, vector3df(x, 0.f, y));
				cube->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
				cube->setMaterialTexture(0, wallAltTexture);
				wallCollisions[x][y] = true;
			}
		}
	}
	delete map;

	camera = smgr->addCameraSceneNode(smgr->getRootSceneNode(), vector3df(1.0f, 0.f, 1.f), vector3df(2.0f, 0.f, 1.f), 1);
	ISceneNodeAnimator* animator = new CameraAnimator(driver, device->getCursorControl(), 5.0f, 30.f, wallCollisions);
	camera->addAnimator(animator);
	camera->bindTargetAndRotation(true);
	camera->setTarget(vector3df(2.0f, 0.0f, 0.0f));
	animator->drop();
	device->getCursorControl()->setVisible(false);

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
	// Get the camera animator and test click
	ISceneNodeAnimatorList animatorList = camera->getAnimators();
	ISceneNodeAnimator* animator = *(animatorList.begin());
	CameraAnimator* cameraAnimator = static_cast<CameraAnimator*>(animator);
	if (cameraAnimator->getLeftMouseIsJustPressed())
	{
		gunCurrentFrame = 1;
		gunNextFrameCounter = 0.1f;
	}

	// Gun frame change
	if (gunCurrentFrame > 0)
	{
		gunNextFrameCounter -= dt;

		if (gunNextFrameCounter <= 0.0f)
		{
			++gunCurrentFrame;
			if (gunCurrentFrame > 2) gunCurrentFrame = 0;
			gunNextFrameCounter = 0.1f;
		}
	}
}

void draw()
{
	driver->beginScene(true, true, SColor(255, 100, 101, 140));

	smgr->drawAll();
	driver->draw2DImage(gunTextures[gunCurrentFrame], recti(vector2di(490, 420), dimension2di(300, 300)), recti(0, 0, 512, 512), nullptr, nullptr, true);

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

unsigned char* readBitmap(const char* filename)
{
	int i;
	FILE* f;
	fopen_s(&f, filename, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	int width, height;
	memcpy(&width, info + 18, sizeof(int));
	memcpy(&height, info + 22, sizeof(int));

	int heightSign = 1;
	if (height < 0) {
		heightSign = -1;
	}

	int size = 3 * width * abs(height);
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	// Swap between every first and third pixel is done because 
	// windows stores the color values as (B, G, R) triples, not (R, G, B).
	if (heightSign == 1) {

		for (i = 0; i < size; i += 3)
		{
			unsigned char tmp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = tmp;
		}
	}
	return data;
}

template <class T>
T* addEntity<T>(u32 x, u32 y)
{
	T* entity = new T();
	entity->create(smgr, x, y);
	entities.push_back(entity);
	return entity;
}