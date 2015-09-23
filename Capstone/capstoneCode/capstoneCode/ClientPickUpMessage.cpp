#include "ClientPickUpMessage.h"

using namespace C4;


ClientPickUpMessage::ClientPickUpMessage(void) : Message(kDefaultPickUpMessage)
{
}

ClientPickUpMessage::ClientPickUpMessage(MessageType type, long contIndex, unsigned_int32 equipmentType) : Message(type)
{
	equipType = equipmentType;
	controllerIndex = contIndex;
}

ClientPickUpMessage::ClientPickUpMessage(MessageType type) : Message(type)
{
	
}

ClientPickUpMessage::~ClientPickUpMessage()
{
}

void ClientPickUpMessage::Compress(Compressor& data) const
{
}

bool ClientPickUpMessage::Decompress(Decompressor& data)
{
	return (true);
}

bool ClientPickUpMessage::HandleMessage(Player *sender) const
{
	MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(sender)->GetController());
	if (controller)
	{
		MyPlayerController *player2 = static_cast <MyPlayerController *> (TheWorldMgr->GetWorld()->GetController(controllerIndex));

		Point3D Loc = player2->GetTargetNode()->GetWorldPosition();
		SoundMessage sMessage(kMessageSound, kPickupSound, 100.0f, Loc);
		TheMessageMgr->SendMessage(player2->getPlayerKey(), sMessage);

		switch (GetMessageType())
		{

			case kDefaultPickUpMessage:
			{
				break;
			}
			//If the player picked up fuel  
			case kFuelPickUpMessage:
			{
				MyPlayerController *player = static_cast <MyPlayerController *> (TheWorldMgr->GetWorld()->GetController(controllerIndex));	
				player->setFuel(100);
				break;
			}
			//If the player picked up health
			case kHealthPickUpMessage:
			{
				MyPlayerController *player = static_cast <MyPlayerController *> (TheWorldMgr->GetWorld()->GetController(controllerIndex));
				player->setHealth(100);
				break;
			}
			//If the player picked up scraps
			case kScrapPickUpMessage:
			{
				MyPlayerController *player = static_cast <MyPlayerController *> (TheWorldMgr->GetWorld()->GetController(controllerIndex));
				player->setScraps(player->getScraps() + 100);
				break;
			}
			//If the player picked up Armor
			case kArmorPickUpMessage:
			{	//Send message to everyone to tell them what type of armor it was
				AddArmorToInventoryMessage message(kMessageAddArmorToInventory, controllerIndex, equipType);
				TheMessageMgr->SendMessageAll(message);
				break;
			}
			//If the player picked up a Weapon
			case kWeaponPickUpMessage:
			{	//Send message to everyone to tell them what type of weapon it was
				AddWeaponToInventoryMessage message(kMessageAddWeaponToInventory, controllerIndex, equipType);
				TheMessageMgr->SendMessageAll(message);
				break;
			}
		}
	}

	return (true);
}