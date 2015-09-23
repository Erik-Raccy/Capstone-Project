#ifndef ENTITYPICKUPS_H
#define ENTITYPICKUPS_H

//Engine Headers

//My Headers
#include "EntityUndamageable.h"


namespace C4
{
	class EntityPickups : public EntityUndamageable
	{
	public:
		//constructors
		EntityPickups();
		EntityPickups(EntityType _type, Point3D _location, char* _model);
		~EntityPickups();

		//Methods
		virtual void OnPickup();

	private:
		float value;
	};
}

#endif