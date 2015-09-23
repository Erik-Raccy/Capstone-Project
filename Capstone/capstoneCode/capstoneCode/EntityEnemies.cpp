#include "EntityEnemies.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityEnemies::EntityEnemies()
{
}

EntityEnemies::EntityEnemies(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, EntityWeapon* _equippedWeapon, EntityArmor* _equippedArmor, char* _model) : EntityMoveable(_type, _location, _health, _maxHealth, _fuel, _maxFuel, _scraps, _equippedWeapon, _equippedArmor, _model)
{
	//nothing unique to entityEnemies currently
}

//default weapon and armor constructor
EntityEnemies::EntityEnemies(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, char* _model) : EntityMoveable(_type, _location, _health, _maxHealth, _fuel, _maxFuel, _scraps, _model)
{

}

EntityEnemies::~EntityEnemies()
{
}

//Inherited Methods
void EntityEnemies::onDamaged()
{
	EntityMoveable::OnDamaged();
}

void EntityEnemies::onDeath()
{
	EntityMoveable::OnDeath();
}

void EntityEnemies::attack()
{
	EntityMoveable::Attack();
}
