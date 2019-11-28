#include "Entity.h"

Entity::Entity()
	:smgr(nullptr), isDestroyed(false), sprite(nullptr)
{}

Entity::~Entity()
{}

void Entity::create(ISceneManager* smgr, u32 x, u32 y)
{
	this->smgr = smgr;
	sprite = smgr->addBillboardSceneNode(nullptr);
	sprite->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
	sprite->setMaterialType(E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
	sprite->setSize(1, 1, 1);
	sprite->setPosition(vector3df(x, 0.0f, y));
}

const bool Entity::getDestroyed() const
{
	return isDestroyed;
}

const vector2df Entity::getPosition() const
{
	return vector2df(sprite->getPosition().X, sprite->getPosition().Z);
}

void Entity::takeDamage(u32 damages)
{}

void Entity::update(const f32 dt)
{}

void Entity::destroy()
{
	isDestroyed = true;
	sprite->remove();
}
