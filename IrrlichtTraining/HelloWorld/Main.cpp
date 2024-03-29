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

	guienv->addStaticText(L"Hello World!", rect<s32>(10, 10, 260, 22), true);

	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 100, 101, 140));

		guienv->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}