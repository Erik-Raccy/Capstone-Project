#ifndef ENTITYWEAPON_H
#define ENTITYWEAPON_H

//Engine Headers

//My Headers
#include "EntityEquippable.h"

namespace C4
{

	class EntityWeapon : public EntityEquippable
	{
	public:
		//constructors
		EntityWeapon();
		EntityWeapon(EntityType eType, Point3D _location, float _damage, float _knockback, float _range, char* _model, EquipmentType _weapon);
		~EntityWeapon();

		//Accesors
		float getDamage()
		{
			return damage;
		}

		float getKnockback()
		{
			return knockback;
		}
		
		float getRange()
		{
			return range;
		}

		unsigned_int32 GetWeaponType()
		{
			return weapon;
		}

		//Methods
		virtual void OnPickup();

	private:
		//Variables
		float damage;
		float knockback;
		float range;
		unsigned_int32 weapon;
	};

}

#endif