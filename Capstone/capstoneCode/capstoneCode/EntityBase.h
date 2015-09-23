#ifndef MYENTITY_H
#define MYENTITY_H

/*
* File:  EntityBase.h
* --------------------------
* This file contains the EntityBase class. All
* entities and controllers in the game are children of this class
* this heirarchy of classes is stored in the PlayerManager's entity 
* list.
*/


//Engine Headers
//My Headers
#include "C4Engine.h"


namespace C4
{
	//All the different types of Entities
	enum EntityType
	{
		kEntityBase = 0,
		kEntityDamageable,
		kEntityMoveable,
		kEntityPlayer,
		kEntityEnemy,
		kEntityUnmovable,
		kEntityUndamageable,
		kEntityPickup,
		kEntityPowerup,
		kEntityArmor,
		kEntityWeapon,
		kEntityFuel,
		kEntityHealth,
		kEntityScraps,
		kEntityProps,
		kEntityBullet
	};

	
	//Base entity class, it is the root of all other entity classes.
	class EntityBase
	{
	public:
		//constructors
		EntityBase();
		EntityBase(EntityType _type, Point3D _location, char* _model);
		~EntityBase();

		//Accessors
		EntityType getType()
		{
			return type;
		}

		unsigned_int32 getID()
		{
			return entityID;
		}

		Point3D GetPosition()
		{
			return location;
		}
		
		void SetPosition(Point3D _position)
		{
			location = _position;
		}

		void SetID(unsigned_int32 _id)
		{
			entityID = _id;
		}
		
	private:
		//Variables
		Point3D location;
		EntityType type;
		unsigned_int32 entityID; //controller ID, only needed for AI
		char* model;
	};

}

#endif