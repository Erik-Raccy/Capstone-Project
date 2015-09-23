#include "PlayerKillRequestMessage.h"

using namespace C4;

PlayerKillRequestMessage::PlayerKillRequestMessage(MessageType type) : Message(type)
{
}

PlayerKillRequestMessage::PlayerKillRequestMessage(MessageType mType, EntityType eType, unsigned_int32 _killContIndex) : Message(mType)
{
	entityType = eType;
	killIndex = _killContIndex;
}

PlayerKillRequestMessage::~PlayerKillRequestMessage()
{
}

void PlayerKillRequestMessage::Compress(Compressor& data) const
{
	Message::Compress(data);
	data << killIndex;
	data << entityType;
}

bool PlayerKillRequestMessage::Decompress(Decompressor& data)
{
	Message::Decompress(data);
	data >> killIndex;
	data >> entityType;
	return (true);
}