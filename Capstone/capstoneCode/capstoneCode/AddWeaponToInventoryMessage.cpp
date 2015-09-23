#include "AddWeaponToInventoryMessage.h"

using namespace C4;


AddWeaponToInventoryMessage::AddWeaponToInventoryMessage(MessageType type, long contIndex, unsigned_int32 equipmentType) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

AddWeaponToInventoryMessage::AddWeaponToInventoryMessage(MessageType type) : Message(type)
{

}
AddWeaponToInventoryMessage::~AddWeaponToInventoryMessage()
{
}

void AddWeaponToInventoryMessage::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex;
}

bool AddWeaponToInventoryMessage::Decompress(Decompressor& data)
{
	int temp;
	data >> equipped;
	data >> temp;

	controllerIndex = temp;
	return (true);
}
//When the player pickups a weapon add it to their inventory
bool AddWeaponToInventoryMessage::HandleMessage(Player *sender) const
{
	Controller *cont = nullptr;
	Node *node = nullptr;
	Player *p = TheMessageMgr->GetFirstPlayer();
	while (p)
	{
		GamePlayer *gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			long  id = cont->GetControllerIndex();
			if (id == controllerIndex){ //If it is the player that pickedup the weapon

				MyPlayerController *player = static_cast<MyPlayerController *>(cont);

				//Default weapon
				float damage = 1;
				float knockback = 1;
				float range = 1;
				EquipmentType type = k_weaponNoWeapon;

				//Find the weapon they picked up
				switch (equipped)
				{
				case k_weaponBlender:
					damage = WEAPON_BLENDER_DAMAGE;
					knockback = WEAPON_BLENDER_KNOCKBACK;
					range = WEAPON_BLENDER_RANGE;
					type = k_weaponBlender;
					break;
				case k_weaponChainsaw:
					damage = WEAPON_CHAINSAW_DAMAGE;
					knockback = WEAPON_CHAINSAW_KNOCKBACK;
					range = WEAPON_CHAINSAW_RANGE;
					type = k_weaponChainsaw;
					break;
				case k_weaponGun:
					damage = WEAPON_ROCKETLAUNCHER_DAMAGE;
					knockback = WEAPON_ROCKETLAUNCHER_KNOCKBACK;
					range = WEAPON_ROCKETLAUNCHER_RANGE;
					type = k_weaponGun;
					break;
				case k_weaponTableSaw:
					break;
				
				}
				//Add that weapon to the player's inventory
				EntityWeapon *weaopon = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), damage, knockback, range, nullptr, type);
				player->AddItemToInventory(weaopon);
			}
		}
		p = p->Next();
	}

	return (true);
}
