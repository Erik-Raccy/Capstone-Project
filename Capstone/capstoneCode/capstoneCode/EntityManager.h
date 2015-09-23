#pragma once

/*
* File:  EntityManager.h
* --------------------------
* This file contains the EntityManager singleton
* this singleton is responsible for ensuring that there 
* are always a certain number of enemies on the map.
* not to be confused with the PlayerManager which handles
* the list of entities.
*/

#include "C4Base.h"
#include "C4Messages.h"
#include "Game.h"

namespace C4
{
	class EntityManager : public Singleton<EntityManager>
	{
	public:
		EntityManager();
		~EntityManager();

		void EnsureEnemies();
		int MAX_ENEMIES;
	private:
		
		float MessageTimer = 0;
	};

	extern EntityManager * TheEntityManager;

}