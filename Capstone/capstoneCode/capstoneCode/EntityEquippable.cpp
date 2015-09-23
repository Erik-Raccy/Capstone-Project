#include "EntityEquippable.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityEquippable::EntityEquippable() : EntityPickups()
{
	equipment = k_ArmorRust; //Default is rust armor
}

EntityEquippable::EntityEquippable(EntityType _type, Point3D _location, char* _model, EquipmentType _equipment) : EntityPickups(_type, _location, _model)
{
	equipment = _equipment;
}


EntityEquippable::~EntityEquippable()
{
}

void EntityEquippable::OnPickup()
{
	EntityPickups::OnPickup();
}
