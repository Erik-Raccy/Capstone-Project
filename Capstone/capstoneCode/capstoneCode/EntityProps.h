#ifndef ENTITYPROPS_H
#define ENTITYPROPS_H

//Engine Headers

//My Headers
#include "EntityDamageable.h"

namespace C4
{
	class EntityProps : public EntityDamageable
	{
	public:
		//constructors
		EntityProps();
		EntityProps(EntityType _type, Point3D _location, float _health, float _maxHealth, char* _model);
		~EntityProps();

	};

}

#endif