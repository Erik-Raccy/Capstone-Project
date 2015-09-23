#include "NewProjectileRequestMessage.h"

using namespace C4;

NewProjectileRequestMessage::NewProjectileRequestMessage() : Message(kNewProjectileRequestMessage)
{

}

NewProjectileRequestMessage::NewProjectileRequestMessage(unsigned_int32 _projType, Point3D p, Vector3D v) : Message(kNewProjectileRequestMessage)
{
	position = p;
	direction = v;
	projType = _projType;
}

NewProjectileRequestMessage::~NewProjectileRequestMessage()
{
}

void NewProjectileRequestMessage::Compress(Compressor &data) const
{
	Message::Compress(data);
	data << position;
	data << direction;
	data << projType;
}

bool NewProjectileRequestMessage::Decompress(Decompressor &data)
{
	if (Message::Decompress(data))
	{
		data >> position;
		data >> direction;
		data >> projType;
		return true;
	}
	return false;
}

