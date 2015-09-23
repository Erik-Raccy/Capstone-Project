#include "DestroyProjectileMessage.h"

using namespace C4;

DestroyProjectileMessage::DestroyProjectileMessage(long contIndex) : ControllerMessage(kDestroyProjectileMessage, contIndex)
{

}

DestroyProjectileMessage::~DestroyProjectileMessage()
{

}

void DestroyProjectileMessage::Compress(Compressor &data) const
{
	ControllerMessage::Compress(data);
}

bool DestroyProjectileMessage::Decompress(Decompressor &data)
{
	if (ControllerMessage::Decompress(data))
	{
		return true;
	}
	return false;
}
