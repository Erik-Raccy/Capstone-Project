#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

//Engine Headers

//My Headers
#include "EntityMoveable.h"
#include "EntityWeapon.h"
#include "EntityArmor.h"
#include <vector>
namespace C4
{
	class EntityPlayer : public EntityMoveable
	{
	public:
		//constructors
		EntityPlayer();
		//Full Constructor
		EntityPlayer(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, EntityWeapon* _equippedWeapon, EntityArmor* _equippedArmor, char* _model);
		//Constructor
		EntityPlayer(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, char* _model);
		~EntityPlayer();

		//Methods
		virtual void Attack();
		virtual void OnDamaged();
		virtual void OnDeath();

		bool getIsDead()
		{
			return isDead;
		}

		//The equipment that the player currently has
		std::vector<EntityEquippable *> InventoryEquipment;

		//Add and remove equipment from the player's inventory
		void AddItemToInventory(EntityEquippable *equipment);
		void RemoveItemToInventory(unsigned_int32 type);

	private:
		bool isDead;

	};

}

#endif