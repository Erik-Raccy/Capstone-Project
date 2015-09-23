#include "MyPlayerManager.h"
#include "Game.h"
#include "EntityFuel.h"
#include "EntityHealth.h"
#include "EntityArmor.h"
#include "EntityBase.h" 
#include <time.h>
#include <string>
using namespace C4;

MyPlayerManager *C4::ThePlayerManager = nullptr;

MyPlayerManager::MyPlayerManager() : Singleton<MyPlayerManager>(ThePlayerManager)
{
	//initialize random
	srand(time(NULL));
}


MyPlayerManager::~MyPlayerManager()
{

}

//individual spawn player called by the spawn message's
void MyPlayerManager::ConstructAvatar(Player *player, Point3D location, int32 controllerIndex)
{
	
}

//called by the server to spawn the player list to the clients
void MyPlayerManager::SendPlayerList(PlayerEvent event, Player *player, const void *param)
{
	GamePlayer *gp = nullptr;
	MyPlayerController *cont = nullptr;
	Node *node = nullptr;
	PlayerKey key = -1;
	long id = -1;
	Point3D loc;
	
	// This player just joined, so send him 1 message per player to spawn a character for each
	Player *p = TheMessageMgr->GetFirstPlayer();
	while (p)
	{
		if (p != player) //this player will already have an avatar, we don't need to send him another
		{
			gp = static_cast<GamePlayer *>(p);
			cont = static_cast<MyPlayerController *>(gp->GetController());
			if (cont)
			{
				node = cont->GetTargetNode();
				key = gp->GetPlayerKey();
				id = cont->GetControllerIndex();
				loc = node->GetWorldPosition();
				TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnMessage(key, id, loc));

			}
		}
		p = p->Next();
	}

	//While loop to set the armor and the weapon for each player in the game
	Player *p2 = TheMessageMgr->GetFirstPlayer();
	Controller *cont2 = nullptr;
	while (p2)
	{
		GamePlayer *gp = static_cast<GamePlayer *>(p2);
		cont2 = gp->GetController();

		if (cont)
		{
			MyPlayerController *spawner = static_cast<MyPlayerController *>(gp->GetController());
			MyWorld *world = static_cast<MyWorld *>(TheWorldMgr->GetWorld());
			
			if (spawner)
			{
				unsigned_int32 armorType = spawner->getArmor()->GetArmorType();
				unsigned_int32 weapon = spawner->getWeapon()->GetWeaponType();

				// Semd message with the other player's armor and weapon
				//TheMessageMgr->SendMessage(player->GetPlayerKey(), AddArmorAndWeaponToPlayersMessage(spawner->GetControllerIndex(), weapon,armorType));
			}
		}
		p2 = p2->Next();
	}

	//also send the list of AIs and Pickups HERE
	for (unsigned_int32 i = 0; i < EntityList.size(); i++)
	{
		//Spawn all AI'
		if (EntityList.at(i)->getType() == kEntityEnemy)
		{
			int32 contIndex = EntityList.at(i)->getID();
			Point3D loc = EntityList.at(i)->GetPosition();
		
			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnAIMessage(contIndex, loc));
		}

		/* 
		/* PICKUPS
		*/

		//Health pickups
		else if ((EntityList.at(i)->getType() == kEntityHealth))
		{
			HealthPickUpController *controller = static_cast<HealthPickUpController *>(EntityList.at(i));
			int32 contIndex = controller->GetControllerIndex();
			Point3D SpawnLoc = controller->GetTargetNode()->GetNodePosition();
			int32 type = kEntityHealth;

			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnDropMessage(contIndex, SpawnLoc, type, NULL));
		}

		//Fuel pickups
		else if ((EntityList.at(i)->getType() == kEntityFuel))
		{
			FuelPickUpController *controller = static_cast<FuelPickUpController *>(EntityList.at(i));
			int32 contIndex = controller->GetControllerIndex();
			Point3D SpawnLoc = controller->GetTargetNode()->GetNodePosition();
			int32 type = kEntityFuel;

			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnDropMessage(contIndex, SpawnLoc, type, NULL));
		}

		//Scrap pickups
		else if ((EntityList.at(i)->getType() == kEntityScraps))
		{
			ScrapPickUpController *controller = static_cast<ScrapPickUpController *>(EntityList.at(i));
			int32 contIndex = controller->GetControllerIndex();
			Point3D SpawnLoc = controller->GetTargetNode()->GetNodePosition();
			int32 type = kEntityScraps;

			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnDropMessage(contIndex, SpawnLoc, type, NULL));
		}

		//Armor pickups
		else if ((EntityList.at(i)->getType() == kEntityArmor))
		{
			ArmorPickUpController *controller = static_cast<ArmorPickUpController *>(EntityList.at(i));
			int32 contIndex = controller->GetControllerIndex();
			Point3D SpawnLoc = controller->GetTargetNode()->GetNodePosition();
			int32 type = kEntityArmor;

			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnDropMessage(contIndex, SpawnLoc, type, controller->GetArmorType()));
		}

		//Weapon pickups
		else if ((EntityList.at(i)->getType() == kEntityWeapon))
		{
			WeaponPickUpController *controller = static_cast<WeaponPickUpController *>(EntityList.at(i));
			int32 contIndex = controller->GetControllerIndex();
			Point3D SpawnLoc = controller->GetTargetNode()->GetNodePosition();
			int32 type = kEntityWeapon;

			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnDropMessage(contIndex, SpawnLoc, type, controller->GetWeaponType()));
		}

		//Bullet entities
		else if ((EntityList.at(i)->getType() == kEntityBullet))
		{
			MyProjectileController *controller = static_cast<MyProjectileController *>(EntityList.at(i));
			int32 contIndex = controller->GetControllerIndex();
			Point3D SpawnLoc = controller->GetTargetNode()->GetNodePosition();
			int32 type = kEntityBullet;

			TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnProjectileMessage(contIndex, controller->GetProjType(), SpawnLoc,controller->GetDirection()));
		}
	}	
}

//called by the server to spawn an avatar
void MyPlayerManager::SpawnNewPlayer(Player *from, bool respawning)
{
	if (TheMessageMgr->Server())
	{
		Point3D loc = GetSpawnMarkerLocation(TheWorldMgr->GetWorld()->GetRootNode()); //THIS FUNCTION NEEDS TO BE CALLED ONCE
		
		//ANY CHECKS TO SEE IF YOU DON"T WANT AN AVATAR GO BEFORE HERE
		int32 contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
		
		// DEBUG CODE
		//String<> text;
		//text += "Player ID : ";
		//text += std::to_string(contIndex).c_str();
		//Engine::Report(text);

		TheMessageMgr->SendMessageAll(SpawnMessage(from->GetPlayerKey(), contIndex, loc));
		if (!respawning)
		{
			try
			{
				DataBaseType data = TheGame->myDatabase->at(from->GetPlayerAddress().GetAddress());

				GamePlayer * p = static_cast<GamePlayer *>(from);
				MyPlayerController * cont = static_cast<MyPlayerController *>(p->GetController());
				cont->setScraps(data.GetScraps());
				cont->setFuel(data.GetFuel());
				cont->setHealth(data.GetHealth());
				
				Engine::Report("reloading player from database");
				for (int i = 0; i < data.inventory.size(); i++)
				{
					EntityEquippable * equipment = data.inventory.at(i);

					if (equipment->getType() == kEntityArmor)
					{
						AddArmorToInventoryMessage msg(kMessageAddArmorToInventory, cont->GetControllerIndex(), equipment->getEquipmentName());
						TheMessageMgr->SendMessageAll(msg);
					}
					else if (equipment->getType() == kEntityWeapon)
					{
						AddWeaponToInventoryMessage msg(kMessageAddWeaponToInventory, cont->GetControllerIndex(), equipment->getEquipmentName());
						TheMessageMgr->SendMessageAll(msg);
					}
				}

			}
			catch (std::out_of_range& oor)
			{
				Engine::Report("A new player has joined the game...");
			}
		}
	}
}


//called by the player at the server's request
void MyPlayerManager::SpawnAvatarAtLocation(Player *player, Point3D location, int32 controllerIndex)
{
	World *world = TheWorldMgr->GetWorld();
	if (world)
	{
		GamePlayer *gPlayer = static_cast<GamePlayer *>(player);
		MyPlayerController *cont = static_cast<MyPlayerController *>(gPlayer->GetController());

		//Attach model to player and spawn
		cont = new MyPlayerController();
		cont->SetControllerIndex(controllerIndex);
		gPlayer->SetController(cont, kMyPlayerController);
		Model *model = Model::Get(kModelRobot);
		model->SetController(cont);
		model->SetNodePosition(location);
		world->AddNewNode(model);
		
		//DEBUG CODE
		//if (!TheMessageMgr->Server())
		//{	
		//	String<> text;
		//	text += "Recieved-Avatar ID : ";
		//	text += std::to_string(controllerIndex).c_str();
		//	Engine::Report(text);
		//}

		//ADD TO LIST OF ENTITIES
		EntityList.push_back(cont);
	}
}

//locally spawns a bot.
void MyPlayerManager::SpawnBot(Point3D location, int32 controllerIndex)
{
	World *world = TheWorldMgr->GetWorld();
	if (world)
	{
		MyAIController *controller = new MyAIController();
		controller->SetControllerIndex(controllerIndex);
		Model *model = Model::Get(kModelAI); //gets registered model
		model->SetController(controller);// give it a controller
		model->SetNodePosition(location);
		world->AddNewNode(model);
		controller->SetID(controllerIndex);
		EntityList.push_back(controller);

		////DEBUG CODE
		//if (!TheMessageMgr->Server())
		//{
		//	String<> text;
		//	text += "Recieved-Bot ID : ";
		//	text += std::to_string(controllerIndex).c_str();
		//	Engine::Report(text);
		//}
	}
}

const Point3D MyPlayerManager::GetSpawnMarkerLocation(const Zone *zone)
{
	if (SpawnList.empty())
	{
		//populate the list
		const Marker *marker = zone->GetFirstMarker();
		while (marker)
		{
			if (marker->Enabled())
			{
				const MarkerType type = marker->GetMarkerType();
				if (type == kMarkerLocator)
				{
					const LocatorMarker *locator = static_cast<const LocatorMarker *>(marker);
					if (locator->GetLocatorType() == kMarkerSpawn)
					{
						Point3D loc = marker->GetWorldPosition();
						SpawnList.push_back(loc);
					}
					if (locator->GetLocatorType() == kMarkerAISpawn)
					{
						Point3D loc = marker->GetWorldPosition();
						AISpawnList.push_back(loc);
					}
					if (locator->GetLocatorType() == kMarkerPickupNode)
					{
						Point3D loc = marker->GetWorldPosition();
						PickupSpawnList.push_back(loc);
					}
					if (locator->GetLocatorType() == kMarkerWeaponNode)
					{
						Point3D loc = marker->GetWorldPosition();
						WeaponSpawnList.push_back(loc);
					}
				}
				else if (type == kMarkerPath)
				{
					const PathMarker *path = static_cast<const PathMarker *>(marker);
					if (path->GetMarkerType() == kMarkerPath)
					{
						Point3D loc = marker->GetWorldPosition();
						PathNodeList.push_back(loc);
					}
				}
			}
			marker = marker->Next();
		}
	}

	// Iterate through all of the markers in the zone.
	int range = Math::Random(SpawnList.size());
	Point3D loc = SpawnList.at(range);

	return (loc);
}

//Removes entity from entityList
void MyPlayerManager::RemoveEntity(int32 controllerIndex)
{
	GamePlayer *gp = nullptr;
	MyPlayerController *cont = nullptr;

	for (int i = 0; i < EntityList.size(); i++)
	{
		if (EntityList[i]->getType() == kEntityPlayer)
		{
			MyPlayerController * player = static_cast<MyPlayerController*>(EntityList[i]);
			if (player->GetControllerIndex() == controllerIndex)
			{
				EntityList.erase(EntityList.begin() + i);
			}
		}
	}
}

//Finds entity from entityList
EntityBase * MyPlayerManager::FindEntity(int32 controllerIndex)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		if (EntityList[i]->getID() == controllerIndex)
		{
			return EntityList.at(i);
		}
	}
	return nullptr;

}
//Removes player from world
void MyPlayerManager::RemovePlayer(int32 controllerIndex)
{		
	if (TheMessageMgr->Server())
	{

	

		//Remove player from the entity list
		EntityRemoveMessage message(kEntityRemoveMessage, controllerIndex);
		TheMessageMgr->SendMessageAll(message);

		Player * p = TheMessageMgr->GetFirstPlayer();
		while (p)
		{
			GamePlayer * gp = static_cast<GamePlayer *>(p);
			Controller * contb = gp->GetController();
			MyPlayerController * cont = static_cast<MyPlayerController *>(contb);

			if (cont && cont->GetControllerIndex() == controllerIndex) //If this is the correct player
			{


				// spawn a pickup where the player died
				Point3D SpawnLoc = cont->GetTargetNode()->GetWorldPosition();


				//p
				SpawnParticleMessage particleMessage(SpawnLoc, 1000,1);
				TheMessageMgr->SendMessageAll(particleMessage);

				//Send message to destory player

				TheMessageMgr->SendMessageAll(PlayerDestroyMessage(gp->GetController()->GetControllerIndex()));

				/*
				/*  When the Player dies spawn a pickup
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

					else if (randomNum2 < 70)
					{
						weaponType = k_weaponSelfDestructGun;
					}

					//34% that a chainsaw will be chosen
					else if (randomNum2 < 100)
					{
						weaponType = k_weaponChainsaw;
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

				
				int32 contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();

				//DEBUG CODE
				//String<> text;
				//text += "Pickup ID : ";
				//text += std::to_string(contIndex).c_str();
				//Engine::Report(text);

				//Spawn the pickup
				TheMessageMgr->SendMessageAll(SpawnDropMessage(contIndex, SpawnLoc, type, subType));
				
				//Respawn the player that died
				SpawnNewPlayer(p, true);
			}
			p = p->Next();
		}
	}
}

//Find closes entity for AI
const float INF = 999999;
EntityBase * MyPlayerManager::GetClosestEntity(Vector3D _pos)
{
	float temp = INF;
	unsigned_int32 i = 0;
	int closest = -1;

	while(i < EntityList.size())
	{
		Vector3D delta = _pos - EntityList.at(i)->GetPosition();
		float dist = Sqrt(delta.x*delta.x + delta.y*delta.y + delta.z*delta.z);
		if (dist < temp && EntityList.at(i)->getType() == kEntityPlayer)
		{
			temp = dist; // new closest found
			closest = i;
		}
		
		i++;
	}
	if (closest == -1)
		return nullptr;
	else
		return EntityList.at(closest);
}

//Spawn pickup 
void MyPlayerManager::SpawnPickup(Point3D location, int32 controllerIndex, int32 entity, int32 subType)
{
	World *world = TheWorldMgr->GetWorld();

	//DEBUG CODE
	//if (!TheMessageMgr->Server())
	//{
	//	String<> text;
	//	text += "Recieved-Pickup ID : ";
	//	text += std::to_string(controllerIndex).c_str();
	//	Engine::Report(text);
	//}

	//Spawn different pickups depending on what was chosen
	switch (entity)
	{
		case (kEntityPowerup):
		{
			PickUpController *controller = new PickUpController();
			Model *model = Model::Get(kModelRobot);

			controller->SetControllerIndex(controllerIndex);
			model->SetNodePosition(location);
			model->SetController(controller);// give it a controller
			world->AddNewNode(model); //add to world
			EntityList.push_back(controller); // add to entity list
			break;
		}

		// Health pickup
		case (kEntityHealth):
		{
			HealthPickUpController *controller = new HealthPickUpController();
			Model *model = Model::Get(kModelToolBox);

			controller->SetControllerIndex(controllerIndex);
			model->SetNodePosition(location);
			model->SetController(controller);// give it a controller
			world->AddNewNode(model); //add to world
			EntityList.push_back(controller); // add to entity list
			break;
		}

		// Fuel pickup
		case (kEntityFuel):
		{
			FuelPickUpController *controller = new FuelPickUpController();
			Model *model = Model::Get(kModelFuelCan);

			controller->SetControllerIndex(controllerIndex);
			model->SetNodePosition(location);
			model->SetController(controller);// give it a controller
			world->AddNewNode(model); //add to world
			EntityList.push_back(controller); // add to entity list
			break;
		}

		// Scraps pickup
		case (kEntityScraps) :
		{
			ScrapPickUpController *controller = new ScrapPickUpController();
			Model *model = Model::Get(kModelIngot);

			controller->SetControllerIndex(controllerIndex);
			model->SetNodePosition(location);
			model->SetController(controller);// give it a controller
			world->AddNewNode(model); //add to world
			EntityList.push_back(controller); // add to entity list
			break;
		}

		// Armor pickup
		case (kEntityArmor):
		{
			//default texture
			char * textureName = "textures/rustMaterial";

			//Find the correct texture for the armor
			switch (subType)
			{
			case k_ArmorBronze:
				textureName = "textures/bronzeMaterial";
				break;
			case k_ArmorIron:
				textureName = "textures/ironMaterial";
				break;
			case k_ArmorSteel:
				textureName = "textures/steelMaterial";
				break;
			case k_ArmorGold:
				textureName = "textures/goldMaterial";
				break;
			case k_ArmorDiamond:
				textureName = "textures/diamondMaterial";
				break;
			case k_ArmorTitanium:
				textureName = "textures/titaniumMaterial";
				break;
			}

			ArmorPickUpController *controller = new ArmorPickUpController(subType);
			Model *model = Model::Get(kModelBodyArmor);
			controller->SetControllerIndex(controllerIndex);
			model->SetNodePosition(location);
			model->SetController(controller);// give it a controller
			world->AddNewNode(model); //add to world
			EntityList.push_back(controller); // add to entity list

			//Set the texture
			Geometry *geom = static_cast<Geometry*>(model->FindNode("_null_"));
			MaterialObject *mat = geom->GetMaterialObject(0)->Clone();
			TextureMapAttribute* texAttribute = static_cast<TextureMapAttribute*>(mat->FindAttribute(kAttributeTextureMap));
			texAttribute->SetTexture(textureName);
			geom->SetMaterialObject(0, mat);
			geom->InvalidateShaderData();
			mat->Release();
			
			break;
		}

		// Weapon pickup
		case (kEntityWeapon) :
		{
			//Default Model name
			unsigned_int32 modelName = NULL;

			//Find the correct model to spawn
			switch (subType)
			{
			case k_weaponBlender:
				modelName = kModelBlenderPickup;
				break;
			case k_weaponChainsaw:
				modelName = kModelChainsawPickup;
				break;
			case k_weaponGun:
				modelName = kModelRocketLauncherPickup;
				break;
			case k_weaponSelfDestructGun:
				modelName = kModelClockLauncherPickup;
				break;
			}

			WeaponPickUpController *controller = new WeaponPickUpController(subType);
			Model *model = Model::Get(modelName);

			controller->SetControllerIndex(controllerIndex);
			model->SetNodePosition(location);
			model->SetController(controller);// give it a controller
			world->AddNewNode(model); //add to world
			EntityList.push_back(controller); // add to entity list
			break;
		}

	}
}

