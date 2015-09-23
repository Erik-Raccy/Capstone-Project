#include "EntityProps.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityProps::EntityProps() : EntityDamageable()
{

}

EntityProps::EntityProps(EntityType _type, Point3D _location, float _health, float _maxHealth, char* _model) : EntityDamageable(_type, _location, _health, _maxHealth, _model)
{
	//nothing unique to entity props as of yet
}


EntityProps::~EntityProps()
{
}
