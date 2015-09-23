#include "ChangeArmorMessage.h"

using namespace C4;

ChangeArmorMessage::ChangeArmorMessage(MessageType type, unsigned_int32 equipmentType,long contIndex) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

ChangeArmorMessage::ChangeArmorMessage(MessageType type) : Message(type)
{

}
ChangeArmorMessage::~ChangeArmorMessage()
{
}

void ChangeArmorMessage::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex; 
}

bool ChangeArmorMessage::Decompress(Decompressor& data)
{
	data >> equipped;
	int temp;
	data >> temp;

	controllerIndex = temp; 
	return (true);
}

bool ChangeArmorMessage::HandleMessage(Player *sender) const
{
	//Send the armor update message to everyone
	ChangeArmorMessageAll message(kMessageUpdateArmorAll,controllerIndex, equipped); 
	TheMessageMgr->SendMessageAll(message);
	return (true);
}