#include "ServerHealthUpdateMessage.h"
#include <time.h>

using namespace C4;

ServerHealthUpdateMessage::ServerHealthUpdateMessage(MessageType type, long contIndex, float _damage, unsigned_int32 enumID, int32 attackIndex) : Message(kMessageHealthUpdate)
{
	//Engine::Report(Text::FloatToString(_damage));
	damage = _damage;
	controllerIndex = contIndex;
	contollerEnum = enumID;
	attackerIndex = attackIndex;
}

ServerHealthUpdateMessage::ServerHealthUpdateMessage(MessageType type) : Message(kMessageHealthUpdate)
{
	
	damage = 0;
	controllerIndex = 0;
	contollerEnum = 0;
}

ServerHealthUpdateMessage::~ServerHealthUpdateMessage()
{
	
}

void ServerHealthUpdateMessage::Compress(Compressor& data) const
{
	data << damage;
	data << controllerIndex;
	data << contollerEnum;
	data << attackerIndex;
}

bool ServerHealthUpdateMessage::Decompress(Decompressor& data)
{
	data >> damage;
	int temp;
	data >> temp;
	controllerIndex = temp;
	data >> contollerEnum;
	data >> attackerIndex;
	return true;
}
//Updates the entity's health that got hit
bool ServerHealthUpdateMessage::HandleMessage(Player *sender) const
{
	if (TheMessageMgr->Server())
	{
		float defense;

		//If the entity that got hit was a player
		if (contollerEnum == kEntityPlayer)
		{
			//Engine::Report("Player");
			MyPlayerController * player = static_cast<MyPlayerController*>(TheWorldMgr->GetWorld()->GetController(controllerIndex));
			defense = player->getArmor()->getArmorValue()/10;

			//decrease the player's health that got attacked
			float updatedHealth = player->getHealth() - (damage / defense);  
			player->setHealth(updatedHealth);
			
			//If the player's health drops below 25% play health critical sound locally
			if (player->getHealth() <= 25)
			{
				Point3D Loc = player->GetTargetNode()->GetWorldPosition();
				SoundMessage sMessage(kMessageSound, kHealthLowSound, 100.0f, Loc);
				TheMessageMgr->SendMessage(player->getPlayerKey(), sMessage);
			}
			//If the player's health hits 0 -> they die
			if (player->getHealth() <= 0)
			{
				//Send the rocket launcher sound to everyone
				Point3D position = player->GetTargetNode()->GetNodePosition();
				SoundMessage sMessage(kMessageSound, kCommunistDeath, 100.0f, position);
				TheMessageMgr->SendMessage(attackerIndex, sMessage);

				//Engine::Report("DEAD");
				player->OnDeath(); 
			}
		}

		//If the entity that got hit was an AI
		else if (contollerEnum == kEntityEnemy)
		{
			MyAIController * Enemy = static_cast<MyAIController*>(TheWorldMgr->GetWorld()->GetController(controllerIndex));
			defense = 1;

			
			//decrease the AI's health that got attacked
			float updatedHealth = Enemy->getHealth() - (damage / defense);  
			Enemy->setHealth(updatedHealth);

			//If the AI's health hits 0 -> they die
			if (Enemy->getHealth() <= 0 && !Enemy->AIDead)
			{
				Enemy->OnDeath(); // the player dies if is zero

				//Send death sound to everyone
				Point3D SpawnLoc = Enemy->GetTargetNode()->GetWorldPosition();
				SoundMessage sMessage(kMessageSound, kAIDeathSound, 100.0f, SpawnLoc);
				TheMessageMgr->SendMessageAll(sMessage);

				//Send destroy message to everyone
				PlayerDestroyMessage defeated(controllerIndex);
				TheMessageMgr->SendMessageAll(defeated);

				//Remove AI from entity list 
				EntityRemoveMessage message(kEntityRemoveMessage, controllerIndex);
				TheMessageMgr->SendMessageAll(message);

				/* 
				/*  When the AI dies spawn a pickup
				*/

				//Randomly decide what to spawn
				int randomNum = rand() % 100;

				//NULL for fuel, health and scraps
				int32 subType = NULL;

				//What type of pickup
				EntityType type;

				//17% chance that the pickup will be fuel
				if (randomNum < 17)
				{
					type = kEntityFuel;
				}

				//17% chance that the pickup will be health
				else if (randomNum < 34)
				{
					type = kEntityHealth;
				}

				//16% chance that the pickup will be scraps
				else if (randomNum < 50)
				{
					type = kEntityScraps;
				}

				//25% chance the pickup will be a weapon
				else if (randomNum < 75)
				{
					//Randomly decide what weapon to spawn
					int randomNum2 = rand() % 100;
					unsigned_int32 weaponType = k_weaponNoWeapon;

					//33% that a blender will be chosen
					if (randomNum2 < 10)
					{
						weaponType = k_weaponBlender;
					}
					//33% that a rocket launcher will be chosen
					else if (randomNum2 < 40) 
					{
						weaponType = k_weaponGun;
					}

					//34% that a chainsaw will be chosen
					else if (randomNum2 < 70) 
					{
						weaponType = k_weaponChainsaw;
					}
					//34% that a chainsaw will be chosen
					else if (randomNum2 < 100)
					{
						weaponType = k_weaponSelfDestructGun;
					}

					type = kEntityWeapon;
					subType = weaponType;
				}

				//25% chance the pickup will be armor
				else
				{
					//Randomly decide what armor to spawn
					int randomNum2 = rand() % 100;

					unsigned_int32 armorType = k_ArmorRust;

					//27% that bronze armor will be chosen
					if (randomNum2 < 27) 
					{
						armorType = k_ArmorBronze;
					}

					//23% that iron armor will be chosen
					else if (randomNum2 < 50) 
					{
						armorType = k_ArmorIron;
					}

					//20% that steel armor will be chosen
					else if (randomNum2 < 70) 
					{
						armorType = k_ArmorSteel;
					}

					//15% that gold armor will be chosen
					else if (randomNum2 < 85) 
					{
						armorType = k_ArmorGold;
					}

					//10% that diamond armor will be chosen
					else if (randomNum2 < 95) 
					{
						armorType = k_ArmorDiamond;
					}

					//5% that titanium armor will be chosen
					else if (randomNum2 < 100) 
					{
						armorType = k_ArmorTitanium;
					}

					type = kEntityArmor;
					subType = armorType;
				}

				//Spawn the pickup
				int32 contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
				TheMessageMgr->SendMessageAll(SpawnDropMessage(contIndex, SpawnLoc, type,subType));
			}
		}
	}
	return (true);
}
