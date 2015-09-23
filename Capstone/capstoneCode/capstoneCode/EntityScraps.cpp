#include "EntityScraps.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityScraps::EntityScraps() : EntityPowerups()
{

}

EntityScraps::EntityScraps(EntityType _type, float _value, Point3D _location, char* _model) : EntityPowerups(_type, _value,_location, _model)
{

}


EntityScraps::~EntityScraps()
{
}

void EntityScraps::OnPickup()
{
	EntityPickups::OnPickup();
}
