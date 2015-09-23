#include "EntityRemoverMessage.h"

using namespace C4;


EntityRemoveMessage::EntityRemoveMessage(MessageType type, long contIndex) : Message(type)
{
	controllerIndex = contIndex;
}

EntityRemoveMessage::EntityRemoveMessage(MessageType type) : Message(type)
{

}
EntityRemoveMessage::~EntityRemoveMessage()
{
}

void EntityRemoveMessage::Compress(Compressor& data) const
{
	data << controllerIndex;
}

bool EntityRemoveMessage::Decompress(Decompressor& data)
{
	int temp;
	data >> temp;

	controllerIndex = temp;
	return (true);
}

bool EntityRemoveMessage::HandleMessage(Player *sender) const
{
	//Remove an entity from the entity list
	ThePlayerManager->RemoveEntity(controllerIndex);
	return (true);
}