#include "EntityDamageable.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityDamageable::EntityDamageable() : EntityBase()
{

}

EntityDamageable::EntityDamageable(EntityType _type, Point3D _location, float _health, float _maxHealth,  char* _model) : EntityBase(_type, _location, _model)
{
	mHealth = _health;
	mTotalHealth = _maxHealth;
}


EntityDamageable::~EntityDamageable()
{
}


void EntityDamageable::OnDamaged()
{
}

void EntityDamageable::OnDeath()
{
}
