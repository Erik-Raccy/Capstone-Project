#ifndef ENTITYPOWERUPS_H
#define ENTITYPOWERUPS_H

//Engine Headers

//My Headers
#include "EntityPickups.h"

namespace C4
{
	class EntityPowerups : public EntityPickups
	{
	public:
		//constructors
		EntityPowerups();
		EntityPowerups(EntityType _type, float _value, Point3D _location, char* _model);
		~EntityPowerups();

		//Methods
		virtual void OnPickup();

		//Accesors
		float getValue(){
			return value;
		}

	private:
		float value;
	};

}

#endif