#ifndef ENTITYSCRAPS_H
#define ENTITYSCRAPS_H

//Engine Headers

//My Headers
#include "EntityPowerups.h"

namespace C4
{
	class EntityScraps : public EntityPowerups
	{
	public:
		//constructors
		EntityScraps();
		EntityScraps(EntityType _type, float _value, Point3D _location, char* _model);
		~EntityScraps();

		//Methods
		virtual void OnPickup();

	private:

	};

}

#endif