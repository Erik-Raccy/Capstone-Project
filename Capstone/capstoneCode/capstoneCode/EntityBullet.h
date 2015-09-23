#ifndef ENTITYBULLET_H
#define ENTITYBULLET_H

//Engine Headers

//My Headers
#include "EntityDamageable.h"

namespace C4
{
	class EntityBullet : public EntityBase
	{
	public:
		//constructors
		EntityBullet();
		EntityBullet(EntityType _type);
		~EntityBullet();
	};
}

#endif