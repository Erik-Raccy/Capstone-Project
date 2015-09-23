#include "PlayerMovementMessage.h"

using namespace C4;

NodeStateMessage::NodeStateMessage(ControllerMessageType type, const int32 controllerIndex) : ControllerMessage(type, controllerIndex)
{
}

NodeStateMessage::NodeStateMessage(ControllerMessageType type, const int32 controllerIndex, const Point3D& position, const Vector3D& velocity) : ControllerMessage(type, controllerIndex)
{
	initialPosition = position;
	initialVelocity = velocity;
}

NodeStateMessage::~NodeStateMessage()
{
}

void NodeStateMessage::Compress(Compressor& data) const
{
	ControllerMessage::Compress(data);

	data << initialPosition;
	data << initialVelocity;
}

bool NodeStateMessage::Decompress(Decompressor& data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> initialPosition;
		data >> initialVelocity;
		return (true);
	}

	return (false);
}





PlayerMovementMessage::PlayerMovementMessage(ControllerMessageType type, const int32 controllerIndex) : NodeStateMessage(type, controllerIndex)
{

}

//FIX ME ? (flag)
PlayerMovementMessage::PlayerMovementMessage(ControllerMessageType type, const int32 controllerIndex, const Point3D& position, const Vector3D& velocity, unsigned_int32 flag) : NodeStateMessage(type, controllerIndex, position, velocity)
{
	movementFlag = flag;
}

PlayerMovementMessage::~PlayerMovementMessage()
{
}

void PlayerMovementMessage::Compress(Compressor& data) const
{
	NodeStateMessage::Compress(data);

	data << movementFlag;
}

bool PlayerMovementMessage::Decompress(Decompressor& data)
{
	if (NodeStateMessage::Decompress(data))
	{
		unsigned_int32 flag;

		data >> flag;
		movementFlag = flag;
		return (true);
	}

	return (false);
}





//FIX ME ? (flag)
AIMovementMessage::AIMovementMessage(ControllerMessageType type, const int32 controllerIndex) : NodeStateMessage(type, controllerIndex)
{
}

AIMovementMessage::AIMovementMessage(ControllerMessageType type, const int32 controllerIndex, const Point3D& position, const Vector3D& velocity) : NodeStateMessage(type, controllerIndex, position, velocity)
{
	
}

AIMovementMessage::~AIMovementMessage()
{
}

void AIMovementMessage::Compress(Compressor& data) const
{
	NodeStateMessage::Compress(data);

}

bool AIMovementMessage::Decompress(Decompressor& data)
{
	if (NodeStateMessage::Decompress(data))
	{
		return (true);
	}

	return (false);
}