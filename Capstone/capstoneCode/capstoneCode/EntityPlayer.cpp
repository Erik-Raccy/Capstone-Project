#include "EntityPlayer.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityPlayer::EntityPlayer()
{
}

EntityPlayer::EntityPlayer(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, EntityWeapon* _equippedWeapon, EntityArmor* _equippedArmor, char* _model) : EntityMoveable(_type, _location, _health, _maxHealth, _fuel, _maxFuel, _scraps, _equippedWeapon, _equippedArmor, _model)
{
	//nothing unique to entityPlayer currently
}

EntityPlayer::EntityPlayer(EntityType _type, Point3D _location, float _health, float _maxHealth, float _fuel, float _maxFuel, float _scraps, char* _model) : EntityMoveable(_type, _location, _health, _maxHealth, _fuel, _maxFuel, _scraps, _model)
{

}

EntityPlayer::~EntityPlayer()
{
}

//Add an item to the players inventory 
void EntityPlayer::AddItemToInventory(EntityEquippable *equipment)
{
	InventoryEquipment.push_back(equipment);
}

//Removes an item from the players inventory (when they press the drop button
void EntityPlayer::RemoveItemToInventory(unsigned_int32  type)
{
	for (int i = 0; i < InventoryEquipment.size(); i++)
	{
		if (InventoryEquipment.at(i)->getEquipmentName() == type)
		{
			InventoryEquipment.erase(InventoryEquipment.begin() + i);
			return;
		}
	}
}

//Inherited Methods
void EntityPlayer::OnDamaged()
{
	EntityMoveable::OnDamaged();
}

void EntityPlayer::OnDeath()
{
	EntityMoveable::OnDeath();
}

void EntityPlayer::Attack()
{
	EntityMoveable::Attack();
}
