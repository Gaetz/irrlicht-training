#include <irrlicht.h>

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

	IMeshSceneNode* cube = smgr->addCubeSceneNode(1, nullptr, 0, vector3df(2.0f, 0.f, 0.f), vector3df(0.0f, 45.f, 0.f));
	cube->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
	ICameraSceneNode* camera = smgr->addCameraSceneNode(nullptr, vector3df(0.0f, 0.f, 0.f), vector3df(2.0f, 0.f, 0.f));


	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 100, 101, 140));

		smgr->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}