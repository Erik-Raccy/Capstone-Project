#include "ClientMovementMessage.h"

using namespace C4;


ClientMovementMessage::ClientMovementMessage(MessageType type) : Message(type)
{
}

ClientMovementMessage::ClientMovementMessage(MessageType type, unsigned_int32 flag) : Message(type)
{
	movementFlag = flag;
}

ClientMovementMessage::~ClientMovementMessage()
{
}

void ClientMovementMessage::Compress(Compressor& data) const
{
	data << (unsigned_int32)movementFlag;
}

bool ClientMovementMessage::Decompress(Decompressor& data)
{
	unsigned_int32	flag;

	data >> flag;
	movementFlag = flag;

	return (true);
}

bool ClientMovementMessage::HandleMessage(Player *sender) const
{
	MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(sender)->GetController());
	if (controller)
	{
		switch (GetMessageType())
		{
		case kMessageMovementBegin:
			controller->BeginMovement(movementFlag);
			break;

		case kMessageMovementEnd:
			controller->EndMovement(movementFlag);
			break;
		}
	}

	return (true);
}