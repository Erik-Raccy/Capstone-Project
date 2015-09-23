#ifndef ENTITYMOVEABLE_H
#define ENTITYMOVEABLE_H

//Engine Headers

//My Headers
#include "EntityDamageable.h"
#include "EntityWeapon.h"
#include "EntityArmor.h"

namespace C4
{
	class EntityMoveable : public EntityDamageable
	{
	public:
		//constructors
		EntityMoveable();
		//Full Contructor
		EntityMoveable(EntityType _type, Point3D _location, float _health, float _maxHealth,  float _fuel, float _maxFuel, float _scraps, EntityWeapon* _equippedWeapon, EntityArmor* _equippedArmor, char* _model);
		//Constructor
		EntityMoveable(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, char* _model);
		~EntityMoveable();

		//Methods
		virtual void Attack();
		virtual void OnDamaged();
		virtual void OnDeath();

		//Accesors 
		float getFuel()
		{
			return fuel;
		}

		void setFuel(float newFuel)
		{
			fuel = newFuel;
		}

		float getMaxFuel()
		{
			return mFuel;
		}

		void setMaxFuel(float newMaxFuel)
		{
			mFuel = newMaxFuel;
		}

		float getScraps()
		{
			return scraps;
		}

		void setScraps(float newScraps)
		{
			scraps = newScraps;
		}

		EntityWeapon* getWeapon()
		{
			return weapon;
		}
		void setWeapon(EntityWeapon *newWeapon)
		{
			weapon = newWeapon;
		}

		EntityArmor* getArmor()
		{
			return armor;
		}
		void setArmor(EntityArmor *newArmor)
		{
			armor = newArmor;
		}

	protected:
		//Variables
		float fuel;
		float mFuel;
		float scraps;
		EntityWeapon *weapon;
		EntityArmor *armor;
	};

}

#endif