#include "SpawnParticleMessage.h"

using namespace C4;


SpawnParticleMessage::SpawnParticleMessage(Point3D pos, int num) : Message(kSpawnParticleMessage)
{
	position= pos;
	numOfSparks = num;
	noUse = 0;
}

SpawnParticleMessage::SpawnParticleMessage(Point3D pos, int num, int hack) : Message(kSpawnParticleMessage)
{
	position = pos;
	numOfSparks = num;
	noUse = hack;
}

SpawnParticleMessage::SpawnParticleMessage() : Message(kSpawnParticleMessage)
{

}
SpawnParticleMessage::~SpawnParticleMessage()
{
}

void SpawnParticleMessage::Compress(Compressor& data) const
{
	data << position;
	data << numOfSparks;
	data << noUse;
}

bool SpawnParticleMessage::Decompress(Decompressor& data)
{
	data >> position;
	data >> numOfSparks;
	data >> noUse;

	return (true);
}

//Create sparks when collision occurs
bool SpawnParticleMessage::HandleMessage(Player *sender) const
{
	if (noUse == 0)
	{
		SparkParticleSystem *spark = new SparkParticleSystem(numOfSparks);
		spark->SetNodePosition(position+Vector3D(0,0,1));
		TheWorldMgr->GetWorld()->AddNewNode(spark);
	}
	else
	{
		SparkParticleSystem *spark = new SparkParticleSystem(numOfSparks,0.2);
		spark->SetNodePosition(position);
		TheWorldMgr->GetWorld()->AddNewNode(spark);
	}

	return (true);
}
