#include "SpawnMessage.h"

using namespace C4;

SpawnMessage::SpawnMessage() : Message(C4::kMessageSpawn)
{
	this->contIndex = 0;
	this->key = -1;
}

SpawnMessage::SpawnMessage(PlayerKey playerkey, int32 controllerIndex, Point3D location) : Message(C4::kMessageSpawn)
{
	this->contIndex = controllerIndex;
	this->key = playerkey;
	this->loc = location;
}

SpawnMessage::~SpawnMessage()
{
}

//called by the message manager when the message is recieved
bool SpawnMessage::HandleMessage(Player *sender) const
{
	Player *player = TheMessageMgr->GetPlayer(GetPlayerKey());
	ThePlayerManager->SpawnAvatarAtLocation(player, GetLocation(), GetControllerIndex());
	return (true);
}

void SpawnMessage::Compress(Compressor &data) const
{
	data << loc;
	data << contIndex;
	data << key;
}

bool SpawnMessage::Decompress(Decompressor &data)
{
	data >> loc;
	data >> contIndex;
	data >> key;

	return (true);
}



//debugging code for drops
SpawnDropMessage::SpawnDropMessage() : Message(C4::kMessageDropSpawn)
{
	this->contIndex = 0;
}

SpawnDropMessage::SpawnDropMessage(int32 controllerIndex, Point3D location, int32 entity, int32 subType) : Message(C4::kMessageDropSpawn)
{
	this->contIndex = controllerIndex;
	this->loc = location;
	this->entityType = entity;
	this->subType = subType;
}

SpawnDropMessage::~SpawnDropMessage()
{
}

bool SpawnDropMessage::HandleMessage(Player *sender) const
{
	ThePlayerManager->SpawnPickup(loc, contIndex, entityType, subType);

	return (true);
}

void SpawnDropMessage::Compress(Compressor &data) const
{
	data << loc;
	data << contIndex;
	data << entityType;
	data << subType;
}

bool SpawnDropMessage::Decompress(Decompressor &data)
{
	data >> loc;
	data >> contIndex;
	data >> entityType;
	data >> subType;
	return (true);
}


// CODE FOR AI SPAWN
SpawnAIMessage::SpawnAIMessage() : Message(C4::kAISpawnMessage)
{
	this->contIndex = 0;
}

SpawnAIMessage::SpawnAIMessage(int32 controllerIndex, Point3D location) : Message(C4::kAISpawnMessage)
{
	this->contIndex = controllerIndex;
	this->loc = location;
}

SpawnAIMessage::~SpawnAIMessage()
{

}

bool SpawnAIMessage::HandleMessage(Player *sender) const
{
	ThePlayerManager->SpawnBot(loc, contIndex);

	return (true);
}

void SpawnAIMessage::Compress(Compressor &data) const
{
	data << loc;
	data << contIndex;
}

bool SpawnAIMessage::Decompress(Decompressor &data)
{
	data >> loc;
	data >> contIndex;

	return (true);
}