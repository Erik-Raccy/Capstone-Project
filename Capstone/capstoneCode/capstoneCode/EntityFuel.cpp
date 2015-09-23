#include "EntityFuel.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityFuel::EntityFuel() : EntityPowerups()
{

}

EntityFuel::EntityFuel(EntityType _type, float _value, Point3D _location, char* _model) :EntityPowerups(_type, _value, _location, _model)
{
	// no unique fuel attributes
}


EntityFuel::~EntityFuel()
{
}

void EntityFuel::OnPickup()
{
	EntityPickups::OnPickup();
}
