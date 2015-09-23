#include "UpdateWeaponMessage.h"

using namespace C4;


UpdateWeaponMessage::UpdateWeaponMessage(MessageType type, unsigned_int32 equipmentType, long contIndex) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

UpdateWeaponMessage::UpdateWeaponMessage(MessageType type) : Message(type)
{

}
UpdateWeaponMessage::~UpdateWeaponMessage()
{
}

void UpdateWeaponMessage::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex;
}

bool UpdateWeaponMessage::Decompress(Decompressor& data)
{
	data >> equipped;
	int temp;
	data >> temp;

	controllerIndex = temp;
	return (true);
}

//When a player changes weapon
bool UpdateWeaponMessage::HandleMessage(Player *sender) const
{
	//Send message to everyone to update a player's weapon
	UpdateWeaponMessageAll message(kMessageUpdateWeaponAll, controllerIndex, equipped);
	TheMessageMgr->SendMessageAll(message);

	return (true);
}