#include "EntityUndamageable.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityUndamageable::EntityUndamageable() : EntityBase()
{

}

EntityUndamageable::EntityUndamageable(EntityType _type, Point3D _location, char* _model) :EntityBase(_type, _location, _model)
{
	//Nothing unique to undamageable entities
}


EntityUndamageable::~EntityUndamageable()
{
}
