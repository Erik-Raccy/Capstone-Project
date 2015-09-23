#include "EntityArmor.h"

using namespace C4;

//Constructors
EntityArmor::EntityArmor() : EntityEquippable()
{
	armor = k_ArmorRust; //default is rust armor
}

EntityArmor::EntityArmor(EntityType _type, float _value, Point3D _location, char* _model, EquipmentType _armor) : EntityEquippable(_type, _location, _model, _armor)
{
	value = _value;
	armor = _armor;
}

EntityArmor::~EntityArmor()
{
}

void EntityArmor::OnPickup()
{
	EntityEquippable::OnPickup();
}
