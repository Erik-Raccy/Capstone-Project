#ifndef ENTITYNOTMOVEABLE_H
#define ENTITYNOTMOVEABLE_H

//Engine Headers

//My Headers
#include "EntityDamageable.h"

namespace C4
{
	class EntityNotMoveable : public EntityDamageable
	{
	public:
		//constructors
		EntityNotMoveable();
		EntityNotMoveable(EntityType _type, Point3D _location, float _health, float _maxHealth, char* _model);
		~EntityNotMoveable();

		//Methods
		virtual void OnDamaged();
		virtual void OnDeath();

	};

}

#endif