#include "ClientOrientationMessage.h"

using namespace C4;


ClientOrientationMessage::ClientOrientationMessage(void) : Message(kMessageOrientation)
{
}

ClientOrientationMessage::ClientOrientationMessage(float _orientation) : Message(kMessageOrientation)
{
	orientation = _orientation;
}

ClientOrientationMessage::~ClientOrientationMessage()
{
}

void ClientOrientationMessage::Compress(Compressor& data) const
{
	data << orientation;
}

bool ClientOrientationMessage::Decompress(Decompressor& data)
{
	data >> orientation;
	return (true);
}

bool ClientOrientationMessage::HandleMessage(Player *sender) const
{
	MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(sender)->GetController());
	if (controller)
	{
		controller->BeginOrientation(orientation, sender);
	}

	return (true);
}