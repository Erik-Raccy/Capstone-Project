#include "EntityManager.h"
#include <string>
using namespace C4;

EntityManager *C4::TheEntityManager = nullptr;

EntityManager::EntityManager() : Singleton<EntityManager>(TheEntityManager)
{
	MAX_ENEMIES = 3;
}


EntityManager::~EntityManager()
{

}

//ensure there are at least 10 AI in the game
void EntityManager::EnsureEnemies()
{
	MessageTimer += TheTimeMgr->GetDeltaTime();
	if (TheMessageMgr->Server() && MessageTimer >= 1000)
	{
		MessageTimer = 0;
		int counter = 0;
		for (int i = 0; i < ThePlayerManager->EntityList.size(); i++) //for each one in the entity list
		{
			if (ThePlayerManager->EntityList.at(i)->getType() == kEntityEnemy) // if they are a bot
			{
				counter++; //add to counter
			}
		}

		if (counter < MAX_ENEMIES) // there are not enough enemies
		{
			for (int i = 0; i < (MAX_ENEMIES - counter); i++)
			{

				int32 contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
				int range = Math::Random(ThePlayerManager->AISpawnList.size());
				
				//DEBUG CODE
			/*	String<> text;
				text += "AI ID : ";
				text += std::to_string(contIndex).c_str();
				Engine::Report(text);*/
				
				Point3D loc = ThePlayerManager->AISpawnList.at(range);
				TheMessageMgr->SendMessageAll(SpawnAIMessage(contIndex, loc));
			
			}
		}
	}
}