#include "EntityBase.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityBase::EntityBase()
{

}

EntityBase::EntityBase(EntityType _type, Point3D _location, char* _model)
{
	type = _type;
	location = _location;
	model = _model;
}

EntityBase::~EntityBase()
{
}

