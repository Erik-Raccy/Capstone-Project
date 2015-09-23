#ifndef ENTITYSCENERY_H
#define ENTITYSCENERY_H

//Engine Headers

//My Headers
#include "EntityUndamageable.h"

namespace C4
{
	class EntityScenery : public EntityUndamageable
	{
	public:
		//constructors
		EntityScenery();
		EntityScenery(EntityType _type, Point3D _location, char* _model);
		~EntityScenery();

	};

}

#endif