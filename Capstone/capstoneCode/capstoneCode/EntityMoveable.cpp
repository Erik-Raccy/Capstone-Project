#include "EntityMoveable.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityMoveable::EntityMoveable() : EntityDamageable()
{
}

//Full constructor
EntityMoveable::EntityMoveable(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, EntityWeapon* _equippedWeapon, EntityArmor* _equippedArmor, char* _model) : EntityDamageable(_type, _location, _health, _maxHealth, _model)
{
	fuel = _fuel;
	mFuel = _maxFuel;
	scraps = _scraps;
	weapon = _equippedWeapon;
	armor = _equippedArmor;
}

//Default armor and weapon constructor
EntityMoveable::EntityMoveable(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, char* _model) : EntityDamageable(_type, _location, _health, _maxHealth, _model)
{
	fuel = _fuel;
	mFuel = _maxFuel;
	scraps = _scraps;

	weapon = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_NO_WEAPON_DAMAGE, WEAPON_NO_WEAPON_KNOCKBACK, WEAPON_NO_WEAPON_RANGE, nullptr, k_weaponNoWeapon);
	armor = new EntityArmor(kEntityArmor, ARMOR_RUST_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorRust);
}

EntityMoveable::~EntityMoveable()
{
}


//Inherited Methods
void EntityMoveable::OnDamaged()
{
	EntityDamageable::OnDamaged();

}

void EntityMoveable::OnDeath()
{
	EntityDamageable::OnDeath();


}

//
void EntityMoveable::Attack()
{

}
