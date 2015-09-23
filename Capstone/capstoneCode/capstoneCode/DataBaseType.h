#pragma once

/*
* File:  DataBaseType.h
* --------------------------
* This file contains the game Database storage
* class, instances of this class are kept in a map
* and are used to store players inventory, health and fuel 
* when they leave and rejoin
*/

#include "MyMessages.h"
#include <vector>

namespace C4
{
	//saved data per player
	class DataBaseType
	{
	public:
		DataBaseType(float health, float scraps, float fuel);
		~DataBaseType();
		
		void SetHealth(float _health)
		{
			health = _health;
		}
		float GetHealth()
		{
			return health;
		}
		void SetScraps(float _scraps)
		{
			scraps = _scraps;
		}
		float GetScraps()
		{
			return scraps;
		}
		void SetFuel(float _fuel)
		{
			fuel = _fuel;
		}
		float GetFuel()
		{
			return fuel;
		}

		std::vector<EntityEquippable *> inventory;

	private:
		float health;
		float scraps;
		float fuel;
	};
}



