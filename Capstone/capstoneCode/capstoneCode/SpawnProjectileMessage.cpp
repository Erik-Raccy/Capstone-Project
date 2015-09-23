#include "SpawnProjectileMessage.h"

using namespace C4;

SpawnProjectileMessage::SpawnProjectileMessage() : Message(kSpawnProjectileMessage)
{

}

SpawnProjectileMessage::SpawnProjectileMessage(int32 _contIndex, unsigned_int32 _projType, Point3D p, Vector3D v) : Message(kSpawnProjectileMessage)
{
	position = p;
	direction = v;
	contIndex = _contIndex;
	projType = _projType;
}

SpawnProjectileMessage::~SpawnProjectileMessage()
{
}

void SpawnProjectileMessage::Compress(Compressor &data) const
{
	Message::Compress(data);
	data << position;
	data << direction;
	data << contIndex;
	data << projType;
}

bool SpawnProjectileMessage::Decompress(Decompressor &data)
{
	if (Message::Decompress(data))
	{
		data >> position;
		data >> direction;
		data >> contIndex;
		data >> projType;
		return true;
	}
	return false;
}


