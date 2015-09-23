#include "EntityPowerups.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityPowerups::EntityPowerups() : EntityPickups()
{

}

EntityPowerups::EntityPowerups(EntityType _type, float _value, Point3D _location, char* _model) :EntityPickups(_type, _location, _model)
{
	value = _value;
}

EntityPowerups::~EntityPowerups()
{
}

void EntityPowerups::OnPickup()
{
	EntityPickups::OnPickup();
}
