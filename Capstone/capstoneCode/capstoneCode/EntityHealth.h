#ifndef ENTITYHEALTH_H
#define ENTITYHEALTH_H

//Engine Headers

//My Headers
#include "EntityPowerups.h"

namespace C4
{
	class EntityHealth : public EntityPowerups
	{
	public:
		//constructors
		EntityHealth();
		EntityHealth(EntityType _type, float _value, Point3D _location, char* _model);
		~EntityHealth();

		virtual void OnPickup();
	};

}

#endif