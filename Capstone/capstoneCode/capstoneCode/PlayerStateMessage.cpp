#include "PlayerStateMessage.h"

using namespace C4;

PlayerStateMessage::PlayerStateMessage(long contIndex) : ControllerMessage(kNodeMessageState, contIndex)
{
	position.Set(0.0F, 0.0F, 0.0F);
}

PlayerStateMessage::PlayerStateMessage(long contIndex, Point3D p) : ControllerMessage(kNodeMessageState, contIndex)
{
	position = p;
}

PlayerStateMessage::~PlayerStateMessage()
{
}

void PlayerStateMessage::Compress(Compressor &data) const
{
	ControllerMessage::Compress(data);
	data << position;
}

bool PlayerStateMessage::Decompress(Decompressor &data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> position;
		return true;
	}
	return false;
}

