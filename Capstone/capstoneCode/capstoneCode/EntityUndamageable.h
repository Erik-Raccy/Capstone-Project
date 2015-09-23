#ifndef ENTITYUNDAMAGEABLE_H
#define ENTITYUNDAMAGEABLE_H

//Engine Headers

//My Headers
#include "EntityBase.h"

namespace C4
{
	class EntityUndamageable : public EntityBase
	{
	public:
		//constructors
		EntityUndamageable();
		EntityUndamageable(EntityType _type, Point3D _location, char* _model);
		~EntityUndamageable();
	};
}

#endif