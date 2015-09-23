#include "EntityWeapon.h"

using namespace C4;

//Constructors
EntityWeapon::EntityWeapon() : EntityEquippable()
{
	weapon = k_weaponNoWeapon; //default is no weapon
}

EntityWeapon::EntityWeapon(EntityType _type, Point3D _location, float _damage, float _knockback, float _range, char* _model, EquipmentType _weapon) : EntityEquippable(_type, _location, _model, _weapon)
{
	damage = _damage;
	knockback = _knockback;
	weapon = _weapon;
	range = _range;
}


EntityWeapon::~EntityWeapon()
{
}

void EntityWeapon::OnPickup()
{
	EntityEquippable::OnPickup();
}
