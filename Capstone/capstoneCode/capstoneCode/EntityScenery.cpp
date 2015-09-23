#include "EntityScenery.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityScenery::EntityScenery() : EntityUndamageable()
{

}

EntityScenery::EntityScenery(EntityType _type, Point3D _location, char* _model) :EntityUndamageable(_type, _location, _model)
{
	//nothing unique to entity scenery as of yet
}


EntityScenery::~EntityScenery()
{
}
