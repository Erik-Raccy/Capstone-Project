#include "AddArmorToInventoryMessage.h"

using namespace C4;


AddArmorToInventoryMessage::AddArmorToInventoryMessage(MessageType type, long contIndex, unsigned_int32 equipmentType) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

AddArmorToInventoryMessage::AddArmorToInventoryMessage(MessageType type) : Message(type)
{

}
AddArmorToInventoryMessage::~AddArmorToInventoryMessage()
{
}

void AddArmorToInventoryMessage::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex;
}

bool AddArmorToInventoryMessage::Decompress(Decompressor& data)
{
	int temp;
	data >> equipped;
	data >> temp;

	controllerIndex = temp;
	return (true);
}

//When the player picks up an Armor pickup add it to their inventory
bool AddArmorToInventoryMessage::HandleMessage(Player *sender) const
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

			if (id == controllerIndex){ // the player who pickuped the armor

				MyPlayerController *player = static_cast<MyPlayerController *>(cont);

				//Default armor
				float defense = 5;
				EquipmentType type = k_ArmorRust;

				//Find the armor that the player picked up
				switch (equipped)
				{
				case k_ArmorIron:
					defense = ARMOR_IRON_DEFENSE;
					type = k_ArmorIron;
					break;
				case k_ArmorBronze:
					defense = ARMOR_BRONZE_DEFENSE;
					type = k_ArmorBronze;
					break;
				case k_ArmorSteel:
					defense = ARMOR_STEEL_DEFENSE;
					type = k_ArmorSteel;
					break;
				case k_ArmorGold:
					defense = ARMOR_GOLD_DEFENSE;
					type = k_ArmorGold;
					break;
				case k_ArmorDiamond:
					defense = ARMOR_DIAMOND_DEFENSE;
					type = k_ArmorDiamond;
					break;
				case k_ArmorTitanium:
					defense = ARMOR_TITANIUM_DEFENSE;
					type = k_ArmorTitanium;
					break;
				}
				//Add that armor to the player's inventory
				EntityArmor *armor = new EntityArmor(kEntityArmor, defense, Point3D(NULL, NULL, NULL), nullptr, type);
				player->AddItemToInventory(armor);
			}
		}
		p = p->Next();
	}

	return (true);
}
