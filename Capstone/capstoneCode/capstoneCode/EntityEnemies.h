#ifndef ENTITYENEMIES_H
#define ENTITYENEMIES_H

//Engine Headers

//My Headers
#include "EntityMoveable.h"
#include "EntityWeapon.h"
#include "EntityArmor.h"

namespace C4
{
	class EntityEnemies : public EntityMoveable
	{
	public:
		//constructors
		EntityEnemies();
		EntityEnemies(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, EntityWeapon* _equippedWeapon, EntityArmor* _equippedArmor, char* _model);
		
		//default weapon and armor constructor
		EntityEnemies(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, char* _model);
		~EntityEnemies();

		//Methods
		virtual void attack();
		virtual void onDamaged();
		virtual void onDeath();

	private:

	};

}

#endif