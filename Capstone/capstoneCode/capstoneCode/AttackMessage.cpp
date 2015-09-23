#include "AttackMessage.h"

using namespace C4;

AttackMessage::AttackMessage(MessageType type) : Message(type)
{
}

AttackMessage::AttackMessage(MessageType type, float _damage, float _knockback) : Message(type)
{
	damage = _damage;
	knockback = _knockback;
}

AttackMessage::~AttackMessage()
{
}

void AttackMessage::Compress(Compressor& data) const
{
	data << damage;
	data << knockback;
}

bool AttackMessage::Decompress(Decompressor& data)
{
	data >> damage;
	data >> knockback;
	return (true);
}

//Called when the player presses the attack button and does not have 
// the rocket launcher equipped
bool AttackMessage::HandleMessage(Player *sender) const
{
	MyPlayerController *senderController = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(sender)->GetController());
	if (TheMessageMgr->Server())
	{
		CollisionData	data;

		//Get information of player who is attacking
		const Point3D& position = senderController->GetTargetNode()->GetWorldPosition(); 
		Vector3D forwardVector = senderController->GetTargetNode()->GetNodeTransform()[0];

		//Position player is facing
		Point3D p2 = Point3D(position.x + 3 * forwardVector.x, position.y + 3 * forwardVector.y, position.z + 1.4); 
		Point3D p1 = position;
		p1 = Point3D(position.x + forwardVector.x, position.y + forwardVector.y, position.z + forwardVector.z);

		//position = Point3D()//Check to see if another rigid body is within range
		if (TheWorldMgr->GetWorld()->QueryCollision(p1, p2, 0.0F, kCollisionRigidBody, &data, senderController)) 
		{
			//If there is another rigid body -> Do something about it

			//If the rigid body is a player
			MyPlayerController *collisionController = static_cast<MyPlayerController *>(data.rigidBody);
			if (collisionController->getType() == kEntityPlayer)
			{
				//Create sparks
				SpawnParticleMessage particleMessage(data.position, 100);
				TheMessageMgr->SendMessageAll(particleMessage);

				//Send a message to decrease the player's health
				ServerHealthUpdateMessage message(kMessageHealthUpdate, collisionController->GetControllerIndex(), damage, kEntityPlayer, senderController->getPlayerKey());
				TheMessageMgr->SendMessage(kPlayerServer, message);
			}

			MyAIController *collisionControllerAI = static_cast<MyAIController *>(data.rigidBody);

			//If the rigid body is an AI
			if (collisionControllerAI->getType() == kEntityEnemy) 
			{
				//Create sparks
				SpawnParticleMessage particleMessage(data.position, 100);
				TheMessageMgr->SendMessageAll(particleMessage);

				//Send a message to decrease the AI's health
				ServerHealthUpdateMessage message(kMessageHealthUpdate, collisionControllerAI->GetControllerIndex(), damage, kEntityEnemy, NULL);
				TheMessageMgr->SendMessage(kPlayerServer, message);
			}
		}
	}
	return (true);
}