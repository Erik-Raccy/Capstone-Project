#include "EntityNotMoveable.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityNotMoveable::EntityNotMoveable() : EntityDamageable()
{
}

EntityNotMoveable::EntityNotMoveable(EntityType _type, Point3D _location, float _health, float _maxHealth, char* _model) : EntityDamageable(_type, _location, _health, _maxHealth, _model)
{
	//nothing unique to unmoveable entities 
}

EntityNotMoveable::~EntityNotMoveable()
{
}

//Inherited Methods
void EntityNotMoveable::OnDamaged()
{
	EntityDamageable::OnDamaged();

}

void EntityNotMoveable::OnDeath()
{
	EntityDamageable::OnDeath();
}


