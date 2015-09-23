#include "EntityBullet.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityBullet::EntityBullet() : EntityBase(kEntityBullet, Point3D(0,0,0), nullptr)
{

}

EntityBullet::EntityBullet(EntityType _type) : EntityBase(_type, Point3D(0, 0, 0), nullptr)
{
	//nothing unique to bullet as of yet
}


EntityBullet::~EntityBullet()
{
}
