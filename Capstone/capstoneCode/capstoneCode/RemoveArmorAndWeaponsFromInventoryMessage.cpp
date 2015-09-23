#include "RemoveArmorAndWeaponsFromInventoryMessage.h"

using namespace C4;


RemoveArmorAndWeaponsFromInventoryMessage::RemoveArmorAndWeaponsFromInventoryMessage(MessageType type, long contIndex, unsigned_int32 equipmentType) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

RemoveArmorAndWeaponsFromInventoryMessage::RemoveArmorAndWeaponsFromInventoryMessage(MessageType type) : Message(type)
{

}
RemoveArmorAndWeaponsFromInventoryMessage::~RemoveArmorAndWeaponsFromInventoryMessage()
{
}

void RemoveArmorAndWeaponsFromInventoryMessage::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex;
}

bool RemoveArmorAndWeaponsFromInventoryMessage::Decompress(Decompressor& data)
{
	int temp;
	data >> equipped;
	data >> temp;

	controllerIndex = temp;
	return (true);
}

//When the player removes an Armor or weapon remove it to their inventory
bool RemoveArmorAndWeaponsFromInventoryMessage::HandleMessage(Player *sender) const
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

			if (id == controllerIndex){ // the player who removed the armor

				MyPlayerController *player = static_cast<MyPlayerController *>(cont);
				player->RemoveItemToInventory(equipped);
				Engine::Report("DROPPED");
				Engine::Report(Text::IntegerToString(equipped));
			}
		}
		p = p->Next();
	}

	return (true);
}
