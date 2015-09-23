#ifndef MYPLAYERMANAGER_H
#define MYPLAYERMANAGER_H

/*
* File:  MyPlayerManager.h
* --------------------------
* This file contains the player manager class
* this class acts as a pseudo factory for all entity 
* classes that inherit from the entity heirarchy
* it is responsible for ensuring that all connected players
* are synchronized with their controllers being the same.
* This class also maintains the lists of entities, spawn points
* and path nodes.
*/


#include "C4Base.h"
#include <vector>
#include "Game.h"
#include "EntityBase.h"
#include "EntityWeapon.h" 

namespace C4 {

	class MyPlayerManager :	public Singleton<MyPlayerManager>
	{
	public:
		//constructors
		MyPlayerManager();
		~MyPlayerManager();

		//Methods
		void ConstructAvatar(Player *player, Point3D location, int32 controllerIndex);
		void SendPlayerList(PlayerEvent event, Player *player, const void *param);
		void SpawnPickup(Point3D location, int32 controllerIndex, int32 entity, int32 subType);
		void SpawnNewPlayer(Player *from, bool respawning);
		void SpawnAvatarAtLocation(Player *player, Point3D location, int32 controllerIndex);
		void RemovePlayer(int32 controllerIndex);
		void RemoveEntity(int32 controllerIndex);
		void SpawnBot(Point3D location, int32 controllerIndex);
		EntityBase *FindEntity(int32 controllerIndex);
		const Point3D GetSpawnMarkerLocation(const Zone *zone);
		EntityBase * GetClosestEntity(Vector3D _pos);

		//list of Entities
		std::vector<EntityBase *> EntityList;
		std::vector<Point3D> PathNodeList;
		void ConstructPickup(EntityType _type, Point3D _point);
		std::vector<Point3D> AISpawnList;
		std::vector<Point3D> PickupSpawnList;

	private:
		std::vector<Point3D> SpawnList;
		std::vector<Point3D> WeaponSpawnList;
	};

	extern MyPlayerManager * ThePlayerManager;
}

#endif
