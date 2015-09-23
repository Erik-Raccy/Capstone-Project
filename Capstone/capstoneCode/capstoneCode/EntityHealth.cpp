#include "EntityHealth.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityHealth::EntityHealth() : EntityPowerups()
{

}
EntityHealth::EntityHealth(EntityType _type, float _value, Point3D _location, char* _model) : EntityPowerups(_type, _value, _location, _model)
{
	//no unique health attributes
}


EntityHealth::~EntityHealth()
{
}

void EntityHealth::OnPickup()
{
	EntityPowerups::OnPickup();
}
