#ifndef ENTITYEQUIPPABLE_H
#define ENTITYEQUIPPABLE_H

//Engine Headers

//My Headers
#include "EntityPickups.h"


namespace C4
{
	//Enum for the different types of equipment the player can have
	enum EquipmentType
	{
		//weapons
		k_weaponBlender =0,
		k_weaponNoWeapon, //default weapon 
		k_weaponChainsaw,
		k_weaponTableSaw, //This weapon does not exist yet 
		k_weaponGun,
		k_weaponSelfDestructGun,
		k_weaponUppercut,

		//armor
		k_ArmorRust, //default armor
		k_ArmorIron,
		k_ArmorBronze,
		k_ArmorSteel,
		k_ArmorGold,
		k_ArmorDiamond,
		k_ArmorTitanium
	};

	/* WEAPON CONSTANTS */

	//Blender
	const int WEAPON_BLENDER_KNOCKBACK = 50;
	const int WEAPON_BLENDER_DAMAGE = 50;
	const int WEAPON_BLENDER_RANGE = 5;

	//self destruct gun
	const int SELF_DESTRUCT_DAMAGE = 1;

	//Chainsaw
	const int WEAPON_CHAINSAW_KNOCKBACK = 25;
	const int WEAPON_CHAINSAW_DAMAGE = 25;
	const int WEAPON_CHAINSAW_RANGE = 10;

	//Tablesaw -- this weapon does not exist yet 
	const int WEAPON_TABLESAW_KNOCKBACK = 10;
	const int WEAPON_TABLESAW_DAMAGE = 10;
	const int WEAPON_TABLESAW_RANGE = 10;

	//Rocket launcher
	const int WEAPON_ROCKETLAUNCHER_KNOCKBACK = 10;
	const int WEAPON_ROCKETLAUNCHER_DAMAGE = 20;
	const int WEAPON_ROCKETLAUNCHER_RANGE = 10;

	//No weapon (fists) - default
	const int WEAPON_NO_WEAPON_KNOCKBACK = 10;
	const int WEAPON_NO_WEAPON_DAMAGE =10;
	const int WEAPON_NO_WEAPON_RANGE = 10;

	/* ARMOR CONSTANTS */
	const int ARMOR_RUST_DEFENSE = 5;
	const int ARMOR_BRONZE_DEFENSE = 7;
	const int ARMOR_IRON_DEFENSE = 10;
	const int ARMOR_STEEL_DEFENSE = 12;
	const int ARMOR_GOLD_DEFENSE =  15;
	const int ARMOR_DIAMOND_DEFENSE = 17;
	const int ARMOR_TITANIUM_DEFENSE = 20;


	class EntityEquippable : public EntityPickups
	{
	public:
		//constructors
		EntityEquippable();
		EntityEquippable(EntityType _type, Point3D _location, char* _model, EquipmentType equipment);
		~EntityEquippable();

		//Methods
		virtual void OnPickup();

		//Accessor
		unsigned_int32 getEquipmentName()
		{
			return equipment;
		}

	private:
		unsigned_int32 equipment;
	};
	

}

#endif

