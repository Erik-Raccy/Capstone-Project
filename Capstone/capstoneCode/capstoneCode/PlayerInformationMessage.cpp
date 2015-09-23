#include "PlayerInformationMessage.h"

using namespace C4;

PlayerInformationMessage::PlayerInformationMessage(long contIndex, float health, float fuel, float scraps) : ControllerMessage(kPlayerInformationMessage, contIndex)
{
	messageHealth = health;
	messageFuel = fuel;
	messageScraps = scraps;
}

PlayerInformationMessage::~PlayerInformationMessage()
{
}

void PlayerInformationMessage::Compress(Compressor& data) const
{
	ControllerMessage::Compress(data);

	data << messageHealth;
	data << messageFuel;
	data << messageScraps;
}

bool PlayerInformationMessage::Decompress(Decompressor& data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> messageHealth;
		data >> messageFuel;
		data >> messageScraps;
		return (true);
	}

	return (false);
}
