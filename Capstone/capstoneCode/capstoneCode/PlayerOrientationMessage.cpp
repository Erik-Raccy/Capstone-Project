#include "PlayerOrientationMessage.h"

using namespace C4;

PlayerOrientationMessage::PlayerOrientationMessage(long contIndex) : ControllerMessage(kPlayerMessageOrientation, contIndex)
{
}

PlayerOrientationMessage::PlayerOrientationMessage(long contIndex, float _orientation) : ControllerMessage(kPlayerMessageOrientation, contIndex)
{
	orientation = _orientation;
}

PlayerOrientationMessage::~PlayerOrientationMessage()
{
}

void PlayerOrientationMessage::Compress(Compressor& data) const
{
	ControllerMessage::Compress(data);

	data << orientation;
}

bool PlayerOrientationMessage::Decompress(Decompressor& data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> orientation;
		return (true);
	}

	return (false);
}
