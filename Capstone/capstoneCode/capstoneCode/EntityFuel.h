#ifndef ENTITYFUEL_H
#define ENTITYFUEL_H

//Engine Headers

//My Headers
#include "EntityPowerups.h"

namespace C4
{
	class EntityFuel : public EntityPowerups
	{
	public:
		//constructors
		EntityFuel();
		EntityFuel(EntityType _type, float _value, Point3D _location, char* _model);
		~EntityFuel();

		virtual void OnPickup();
	private:

	};

}

#endif