#ifndef ENTITYARMOR_H
#define ENTITYARMOR_H

//Engine Headers

//My Headers
#include "EntityEquippable.h"

namespace C4
{
	class EntityArmor : public EntityEquippable
	{
	public:
		//constructors
		EntityArmor();
		EntityArmor(EntityType _type, float _value, Point3D _location, char* _model, EquipmentType _armor);
		~EntityArmor();

		//Methods
		virtual void OnPickup();

		//Accesors
		float getArmorValue()
		{
			return value;
		}

		unsigned_int32 GetArmorType()
		{
			return armor;
		}

	private:
		float value;
		unsigned_int32 armor;

	};

}

#endif