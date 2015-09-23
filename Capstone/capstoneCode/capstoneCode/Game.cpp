#include "Game.h"
#include "MainMenu.h"
#include <string>

using namespace C4;

// Global pointer to our game object
Game *C4::TheGame = nullptr;

Application *ConstructApplication(void)
{
	return (new Game);
}
//
//reg needs to be initialized
Game::Game(void) : Singleton<Game>(TheGame),
MyPlayerControllerReg(kControllerPlayer, "MyPlayerController"),
MyAIControllerReg(kControllerAI, "MyAIController"),
PickUpControllerReg(kControllerPickUp, "PickUpController"),
FuelPickUpControllerReg(kControllerFuelPickUp, "FuelPickUpController"),
HealthPickUpControllerReg(kControllerHealthPickUp, "HealthPickUpController"),
ScrapPickUpControllerReg(kControllerScrapPickUp, "ScrapPickUpController"),
ArmorPickUpControllerReg(kControllerArmorPickUp, "ArmorPickUpController"),
WeaponPickUpControllerReg(kControllerWeaponPickUp, "WeaponPickUpController"),
sparkParticleSystemReg(kParticleSystemSpark, "Sparks"),

robotModelReg(kModelRobot, nullptr, "Models/Player/Player", kModelPrecache | kModelPrivate, kControllerPlayer),
chainsawModelReg(kModelChainsaw, nullptr, "Models/ChainsawModel", kModelPrecache | kModelPrivate, kControllerPlayer),
blenderModelReg(kModelBlender, nullptr, "Models/BlenderModel", kModelPrecache | kModelPrivate, kControllerPlayer),
aiModelReg(kModelAI, nullptr, "Models/AI/AIBot", kModelPrecache | kModelPrivate, kControllerAI),
fuelCanModelReg(kModelFuelCan, nullptr, "Models/PickUps/FuelCan", kModelPrecache | kModelPrivate),
toolBoxModelReg(kModelToolBox, nullptr, "Models/PickUps/ToolBox", kModelPrecache | kModelPrivate),
ingotModelReg(kModelIngot, nullptr, "Models/PickUps/Ingot", kModelPrecache | kModelPrivate),
bulletModelReg(kModelMissle, nullptr, "Models/PickUps/SpikeBall", kModelPrecache | kModelPrivate),
bodyArmorModelReg(kModelBodyArmor, nullptr, "Models/PickUps/BodyArmor", kModelPrecache | kModelPrivate),
blenderPickupModelReg(kModelBlenderPickup, nullptr, "Models/PickUps/BlenderPickup", kModelPrecache | kModelPrivate),
chainsawPickupModelReg(kModelChainsawPickup, nullptr, "Models/PickUps/ChainsawPickup", kModelPrecache | kModelPrivate),
rocketLauncherModelReg(kModelRocketLauncher, nullptr, "Models/RocketLauncherModel", kModelPrecache | kModelPrivate),
clockLauncherModelReg(kModelClockLauncher, nullptr, "Models/ClockLauncherModel", kModelPrecache | kModelPrivate),
clockLauncherPickupModelReg(kModelClockLauncherPickup, nullptr, "Models/ClockLauncherPickupModel", kModelPrecache | kModelPrivate),
clockModelReg(kModelClock, nullptr, "Models/clock", kModelPrecache | kModelPrivate),
rocketLauncherPickupModelReg(kModelRocketLauncherPickup, nullptr, "Models/PickUps/RocketLauncherPickup", kModelPrecache | kModelPrivate),

cubeModelReg(kModelCube, nullptr, "Models/PickUps/Cube", kModelPrecache | kModelPrivate, kControllerPickUp),

spawnReg(kMarkerSpawn, "Spawn Location"),
aispawnReg(kMarkerAISpawn, "AI Spawn Location"),
pathnodeReg(kMarkerPath, "AI Path Node"),
pickupReg(kMarkerPickupNode, "Pickup Spawn Location"),

MydisplayEventHandler(&HandleDisplayEvent)
{
	TheWorldMgr->SetWorldConstructor(&ConstructWorld); // pass the function pointer of the construct world method
	new MyPlayerManager;
	new EntityManager;
	//for create server, create observer, command and add it to engine
	CommandObserver<Game> *serverObserver = new CommandObserver<Game>(this, &Game::ServerCallback);
	serverCommand = new Command("server", serverObserver);
	TheEngine->AddCommand(serverCommand);

	//for close server, create observer, command and add it to engine
	CommandObserver<Game> *nameObserver = new CommandObserver<Game>(this, &Game::NameCallback);
	nameCommand = new Command("name", nameObserver);
	TheEngine->AddCommand(nameCommand);

	//for join server, create observer, command and add it to engine
	CommandObserver<Game> *joinObserver = new CommandObserver<Game>(this, &Game::JoinCallback);
	joinCommand = new Command("join", joinObserver);
	TheEngine->AddCommand(joinCommand);
	CommandObserver<Game> *spawnPickUpObserver = new CommandObserver<Game>(this, &Game::PickUpCallback);
	spawnPickUpCommand = new Command("pickup", spawnPickUpObserver);
	TheEngine->AddCommand(spawnPickUpCommand);
	CommandObserver<Game> *spawnAIObserver = new CommandObserver<Game>(this, &Game::SpawnAICallback);
	spawnAICommand = new Command("bot", spawnAIObserver);
	TheEngine->AddCommand(spawnAICommand);
	
	//game presentation commands for enable/disabling AI
	CommandObserver<Game> *DisableAIObserver = new CommandObserver<Game>(this, &Game::DisableAICallback);
	disableAICommand = new Command("disablebots", DisableAIObserver);
	TheEngine->AddCommand(disableAICommand);
	//enable AI spawning
	CommandObserver<Game> *EnableAIObserver = new CommandObserver<Game>(this, &Game::EnableAICallback);
	enableAICommand = new Command("enablebots", EnableAIObserver);
	TheEngine->AddCommand(enableAICommand);
	//get all items
	CommandObserver<Game> *GetAllItemsObserver = new CommandObserver<Game>(this, &Game::GetAllItemsCallback);
	GetAllItemsCommand = new Command("godmode", GetAllItemsObserver);
	TheEngine->AddCommand(GetAllItemsCommand);

	//setup inputs with manager
	forwardAction = new MovementAction(kActionForward);
	backwardAction = new MovementAction(kActionBackward);
	leftAction = new MovementAction(kActionLeft);
	rightAction = new MovementAction(kActionRight);
	upAction = new MovementAction(kActionUp);
	attackAction = new MyAttackAction(kActionAttack);
	pauseAction = new MyPauseAction(kActionPause);
	selfDestructAction = new SelfDestructAction(kActionSelfDestruct);

	// These register our new actions with the Input Manager.
	TheInputMgr->AddAction(forwardAction);
	TheInputMgr->AddAction(backwardAction);
	TheInputMgr->AddAction(leftAction);
	TheInputMgr->AddAction(rightAction);
	TheInputMgr->AddAction(upAction);
	TheInputMgr->AddAction(pauseAction);
	TheInputMgr->AddAction(attackAction);
	TheInputMgr->AddAction(selfDestructAction);

	//display event registration
	TheInterfaceMgr->SetInputManagementMode(kInputManagementAutomatic);

	TheDisplayMgr->SetDisplayMode(TheDisplayMgr->GetDisplayWidth(), TheDisplayMgr->GetDisplayHeight(), 8, kDisplayRefreshSync);

	TheMessageMgr->SetPlayerConstructor(&ConstructPlayer, this);

	// Play the main theme of the game 
	music = new Sound;
	WaveStreamer *streamer = new WaveStreamer;
	streamer->AddComponent("Sounds/MainTheme");
	music->Stream(streamer);
	music->SetLoopCount(kSoundLoopInfinite);
	music->SetSoundPriority(0);
	music->Play();

	// Open the main menu
	TheMainWindow->Open();
	
}

Game::~Game(void)
{
	TheMessageMgr->EndGame();
	TheWorldMgr->UnloadWorld(); //unloads world when game finishes up
	TheWorldMgr->SetWorldConstructor(nullptr); //gets rid of world construction function pointer

	//cleanup movement actions
	delete upAction;
	delete rightAction;
	delete leftAction;
	delete backwardAction;
	delete forwardAction;
	delete attackAction;
	delete pauseAction;
	delete selfDestructAction;
	delete serverCommand;
	delete joinCommand;
	delete GetAllItemsCommand;
	delete enableAICommand;
	delete disableAICommand;
	delete myHud;

	delete TheEntityManager;
	delete ThePlayerManager;

	delete myDatabase;

	music->Stop();
	music->Release();

	if (TheMainWindow)
		delete TheMainWindow;
}

World *Game::ConstructWorld(const char *name, void *data)
{
	return(new MyWorld(name));
}

// This function is called when a display event occurs (because we registered it in the Game constructor).
void Game::HandleDisplayEvent(const DisplayEventData *eventData, void *cookie)
{
	if (eventData->eventType == kEventDisplayChange)
	{
		// The screen resolution has changed. Handle accordingly.
	}
}

//rename commands
void Game::NameCallback(Command *command, const char *text)
{
	RequestRenameMessage message(text);
	TheMessageMgr->SendMessage(kPlayerServer, message);
}

EngineResult Game::LoadWorld(const char *name)
{
	WorldResult result = TheWorldMgr->LoadWorld(name);

	if (result == C4::kWorldOkay)
	{
		World *world = TheWorldMgr->GetWorld();
		if (world)
		{
			MyCamera *camera = new MyCamera();
			camera->SetNodePosition(Point3D(10.0F, 10.0F, 10.0F));
			camera->LookAtPoint(Point3D(0.0F, 0.0F, 0.0F));
			camera->Invalidate();
			world->SetCamera(camera);
		}
		TheEngine->Report("Game has begun!");
	}
	myHud = new MyHud();
	TheInterfaceMgr->AddWidget(myHud);

	return result;
}

void Game::UnloadWorld()
{
	TheInterfaceMgr->RemoveWidget(myHud);
	TheWorldMgr->UnloadWorld();
	TheMessageMgr->EndGame();
}

void Game::HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param)
{
	const long * failReason = static_cast<const long*>(param);
	switch (event)
	{
	case kConnectionQueryReceived:
	{
		 Engine::Report("A request for server info recieved");
	}
	case kConnectionAttemptFailed:
	{
		TheEngine->Report("An attempt to connect to a server has failed. ");

		switch (*failReason){
		case kNetworkFailTimeout:
			TheEngine->Report("The remote machine did not respond and the connection attempt timed out. ");
			break;
		case kNetworkFailWrongProtocol:
			TheEngine->Report("The remote machine is not using the same communications protocol that the local machine is using. ");
			break;
		case kNetworkFailNotServer:
			TheEngine->Report("The remote machine is using the correct protocol, but it is not a server. ");
			break;
		case kNetworkFailServerFull:
			TheEngine->Report("The remote machine did not accept the connection because the maximum number of clients have already connected. ");
			break;
		}
	}
	case kConnectionClientOpened:
	{
		Engine::Report("Client Connected.");
		break;
	}
	case kConnectionClientClosed:
	case kConnectionClientTimedOut:
	{
		Engine::Report("Client Connection Closed.");
		break;
	}
	case kConnectionServerAccepted:
	{
		Engine::Report("We are connected");
		TheGame->LoadWorld("hillWorld");
		TheMessageMgr->SendMessage(kPlayerServer, RequestMessage());//request an avatar from the server
		break;
	}
	case kConnectionServerClosed:
	case kConnectionServerTimedOut:
	{
		Engine::Report("Server Connection Closed.");
		UnloadWorld();
		break;
	}
	default:
	{
		Engine::Report("Unexpected network event");
	}
	}
	Application::HandleConnectionEvent(event, address, param);
}

void Game::HandlePlayerEvent(PlayerEvent event, Player *player, const void *param)
{
	switch (event)
	{
		case kPlayerInitialized: //the fi has finished initializing a new player in a multiplayer game
		{
			if (TheMessageMgr->Server())
			{
				TheEngine->Report("Another Player Has Joined");
				ThePlayerManager->SendPlayerList(event, player, param);
			}
			else
			{
				TheEngine->Report("Client init message");
			}

			break;
		}
		case C4::kPlayerDisconnected:
		case C4::kPlayerTimedOut:
		{

			// If a player disconnects or times out, send a message to every other
			//player to delete this soldier.
			GamePlayer *gp = static_cast<GamePlayer *>(player);
			if (TheMessageMgr->Server())
			{
				//database!
				Engine::Report("player added to database");

				unsigned_int32 ip = gp->GetPlayerAddress().GetAddress();
				MyPlayerController * player = static_cast<MyPlayerController *>(gp->GetController());
				DataBaseType data(player->getHealth(), player->getScraps(), player->getFuel());

				data.inventory = player->InventoryEquipment;

				std::pair<unsigned_int32, DataBaseType> tempPear(ip, data);

				std::pair<std::map<unsigned_int32, DataBaseType>::iterator, bool> ret;

				ret = myDatabase->insert(tempPear);
				if (ret.second == false) //key already exists
				{
					myDatabase->erase(ip);
					myDatabase->insert(tempPear);
				}

				TheMessageMgr->SendMessageAll(PlayerDestroyMessage(gp->GetController()->GetControllerIndex()));
			}
			break;
		}
		// We've received a chat. 
		case kPlayerChatReceived:
		{
			if (player == TheMessageMgr->GetLocalPlayer()) //if the local player is me
				break; //dont print the message

			//String<kMaxChatMessageLength + kMaxPlayerNameLength + 2> text(player->GetPlayerName());
			//text += ": ";
			//text += static_cast<const char *>(param);
			//Engine::Report(text, kReportError);
			break;
		}
	}
	Application::HandlePlayerEvent(event, player, param);
}

void Game::HandleGameEvent(GameEvent event, const void *param)
{

}

Message * Game::ConstructMessage(MessageType type, Decompressor &data) const
{
	switch (type)
	{
		case kMessageSpawn:
			return (new SpawnMessage);
		case kEntityRemoveMessage:
			return (new EntityRemoveMessage(type));
		case kMessageDropSpawn:
			return (new SpawnDropMessage);
		case kDefaultPickUpMessage:
			return (new ClientPickUpMessage(type));
		case kFuelPickUpMessage:
			return (new ClientPickUpMessage(type));
		case kHealthPickUpMessage:
			return (new ClientPickUpMessage(type));
		case kArmorPickUpMessage:
			return (new ClientPickUpMessage(type));
		case kMessageRequest:
			return (new RequestMessage);
		case kMessageMovementBegin:
			return (new ClientMovementMessage(type));
		case kMessageMovementEnd:
			return (new ClientMovementMessage(type));
		case kMessageAttack:
			return (new AttackMessage(type));
		case kMessageOrientation:
			return (new ClientOrientationMessage);
		case (kMessageNameChangeRequestMessage) :
			return (new RequestRenameMessage());
		case (kMessageNameChangeMessage) :
			return (new NameChangeMessage());
		case (kAISpawnMessage) :
			return (new SpawnAIMessage());
		case(kMessageUpdateArmor) :
			return (new ChangeArmorMessage(type));
		case(kMessageUpdateArmorAll) :
			return (new ChangeArmorMessageAll(type));
		case(kMessageUpdateWeapon) :
			return (new UpdateWeaponMessage(type));
		case(kMessageUpdateWeaponAll) :
			return (new UpdateWeaponMessageAll(type));
		case(kMessageAddArmorToInventory) :
			return (new AddArmorToInventoryMessage(type));
		case(kMessageAddWeaponToInventory) :
			return (new AddWeaponToInventoryMessage(type));
		case(kAddArmorAndWeaponMessage) :
			return (new AddArmorAndWeaponToPlayersMessage());
		case (kNewProjectileRequestMessage) :
			return (new NewProjectileRequestMessage());
		case (kSpawnProjectileMessage) :
			return (new SpawnProjectileMessage());
		case(kMessageSound) :
			return (new SoundMessage(type));
		case(kSpawnParticleMessage) :
			return (new SpawnParticleMessage());
		case(kRemoveArmorAndWeaponFromInventoryMessage) :
			return (new RemoveArmorAndWeaponsFromInventoryMessage(type));
		case kMessageHealthUpdate:
			return (new ServerHealthUpdateMessage(type));
		case kPlayerKillRequestMessage:
			return (new PlayerKillRequestMessage(type));
		default:
			return nullptr;
	}
}

//translate the messages into actions (or call a method on a class that does do the action)
void Game::ReceiveMessage(Player *from, const NetworkAddress &address, const Message *message)
{
	MessageType type = message->GetMessageType();

	switch (type)
	{
		case kMessageRequest:
		{
			if(TheMessageMgr->Server())
			{
				Engine::Report("avatar request received by server");
				ThePlayerManager->SpawnNewPlayer(from, false);
			}
			else
			{
				Engine::Report("request received by client");
			}
			break;
		}
		case (kMessageNameChangeRequestMessage) :
		{
			const RequestRenameMessage * msg = static_cast<const RequestRenameMessage *>(message);
			Player * player = TheMessageMgr->GetPlayer(from->GetPlayerKey());
			NameChangeMessage ncmsg(NameChangeMessage(msg->GetNewName(), player->GetPlayerKey()));
			TheMessageMgr->SendMessageAll(ncmsg);

			break;
		}
		case (kMessageNameChangeMessage) :
		{
			const NameChangeMessage * msg = static_cast<const NameChangeMessage *> (message);
			Player * player = TheMessageMgr->GetPlayer(msg->GetPlayerKey());
			if (player)
			{
				// we write our confirmation message now, before we do the actual change.
				String<128> str("Player ");
				str += player->GetPlayerName();
				str += " changed name to ";
				str += msg->GetNewName();
				// Update the player's name
				player->SetPlayerName(msg->GetNewName());
				// Confirm to the user
				TheEngine->Report(str, kReportError);
			}
			else {
				// we display a message that the player was not found.
				String<128> str("Player ");
				str += msg->GetPlayerKey();
				str += " not found while trying to update name. =(";
				TheEngine->Report(str, kReportError);
			}
			break;
		}
		case (kNewProjectileRequestMessage) :
		{
			if (TheMessageMgr->Server())
			{
				const NewProjectileRequestMessage * msg = static_cast<const NewProjectileRequestMessage *> (message);
				int32 newContIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
				
				//DEBUG CODE
				//String<> text;
				//text += "Bullet ID : ";
				//text += std::to_string(newContIndex).c_str();
				//Engine::Report(text);
				
				SpawnProjectileMessage newMessage(newContIndex, msg->GetShotType() , msg->GetPosition(), msg->GetDirection());
				TheMessageMgr->SendMessage(kPlayerAll, newMessage);
			}
			break;
		}
		case (kSpawnProjectileMessage) :
		{
			const SpawnProjectileMessage * msg = static_cast<const SpawnProjectileMessage *> (message);	
			MyProjectileController *proj = new MyProjectileController(msg->GetProjType(), msg->GetDirection());
	
			ThePlayerManager->EntityList.push_back(proj);

			proj->SetControllerIndex(msg->GetContIndex());
			Model *shotModel;
			if (msg->GetProjType() == k_weaponSelfDestructGun)
			{
				shotModel = Model::Get(kModelClock);
			}
			else 
			{
				shotModel = Model::Get(kModelMissle); //gets registered cube model
			}
			shotModel->SetNodePosition(msg->GetPosition());
			
			shotModel->SetController(proj);
			if (!TheMessageMgr->Server())
			{
				//DEBUG CODE
				//String<> text;
				//text += "Client-Bullet ID : ";
				//text += std::to_string(msg->GetContIndex()).c_str();
				//Engine::Report(text);
			}
			World *world = TheWorldMgr->GetWorld();
			world->AddNewNode(shotModel);
		
			break;
		}
		case (kPlayerKillRequestMessage) :
		{
			if (TheMessageMgr->Server()){
				const PlayerKillRequestMessage * msg = static_cast<const PlayerKillRequestMessage *> (message);
				unsigned_int32 killContIndex = msg->GetKillIndex();
				int killEntityType = msg->GetEntityType();

				//Engine::Report(Text::IntegerToString(killEntityType));
				if (killEntityType == kEntityPlayer)
				{
					MyPlayerController * player = static_cast<MyPlayerController*>(TheWorldMgr->GetWorld()->GetController(killContIndex));
					if (player != nullptr)
					{
						player->OnDeath();
					}
				}
				else if (killEntityType == kEntityEnemy)
				{
					MyAIController * enemy = static_cast<MyAIController*>(TheWorldMgr->GetWorld()->GetController(killContIndex));
					if (enemy != nullptr)
					{
						enemy->OnDeath();
					}
				}
			}
			break;
		}
	}
}

void Game::HostGame()
{
	//add settings to network manager
	TheNetworkMgr->SetProtocol(kGameProtocol);
	TheNetworkMgr->SetPortNumber(kGamePort);
	TheMessageMgr->BeginMultiplayerGame(true);


	TheEngine->Report(String<>("Initialized. Hosting on: ") + MessageMgr::AddressToString(TheNetworkMgr->GetLocalAddress(), true));

	TheGame->LoadWorld("hillWorld");
	myDatabase = new std::map<unsigned_int32, DataBaseType>;
	TheMessageMgr->SendMessageAll(RequestMessage());
}

//start server command
void Game::ServerCallback(Command *command, const char *text)
{
	HostGame();
}

void Game::JoinGame(String<>  ipAddress)
{
	//add settings to network manager
	TheNetworkMgr->SetProtocol(kGameProtocol);
	TheNetworkMgr->SetPortNumber(0);
	
	TheMessageMgr->BeginMultiplayerGame(false);

	NetworkAddress addr = MessageMgr::StringToAddress(ipAddress);
	addr.SetPort(kGamePort);
	NetworkResult result = TheMessageMgr->Connect(addr);
	TheEngine->Report(String<>("Attempting connection with: ") + MessageMgr::AddressToString(addr, true));

	if (result == kNetworkOkay)
	{
		TheEngine->Report("Network initialized. Waiting on response...");
	}
	else if (result == kNetworkInitFailed)
	{
		TheEngine->Report(String<>("Error Code: ") += result);
	}
}

//connect to server command
void Game::JoinCallback(Command *command, const char *text)
{
	NetworkAddress address = MessageMgr::StringToAddress(text);// Next, we convert the entered parameters into a C4 NetworkAddress. This format is used internally.
	JoinGame(text);
}

void Game::QuitGame()
{
	TheMessageMgr->DisconnectAll();
	TheEngine->Quit();
}

Player *Game::ConstructPlayer(PlayerKey key, void *data)
{
	return (new GamePlayer(key));
}

void Game::PickUpCallback(Command *command, const char *text)
{
	if (TheMessageMgr->Server())
	{
		int32 contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
		int range = Math::Random(ThePlayerManager->AISpawnList.size());
		MyPlayerController *player = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(TheMessageMgr->GetLocalPlayer())->GetController());
		Point3D loc = player->GetTargetNode()->GetNodePosition();
		Vector3D vec = player->GetTargetNode()->GetNodeTransform()[0];
		loc = loc + 2 * vec;
		int32 entity = NULL;
		int32 subEntity = NULL;
		if (text[0] == '1' && text[1] == NULL)
			entity = kEntityHealth;
		else if (text[0] == '2')
			entity = kEntityFuel;
		else if (text[0] == '3')
			entity = kEntityScraps;
		else if (text[0] == '4')
		{
			entity = kEntityWeapon;
			subEntity = k_weaponChainsaw;
		}
		else if (text[0] == '5')
		{
			entity = kEntityWeapon;
			subEntity = k_weaponGun;
		}
		else if (text[0] == '6')
		{
			entity = kEntityWeapon;
			subEntity = k_weaponBlender;
		}
		else if (text[0] == '7')
		{
			entity = kEntityArmor;
			subEntity = k_ArmorIron;
		}
		else if (text[0] == '8')
		{
			entity = kEntityArmor;
			subEntity = k_ArmorSteel;
		}
		else if (text[0] == '9')
		{
			entity = kEntityArmor;
			subEntity = k_ArmorGold;
		}
		else if (text[0] == '1' && text[1] == '0')
		{
			entity = kEntityArmor;
			subEntity = k_ArmorBronze;
		}
		else if (text[0] == '1' && text[1] == '1')
		{
			entity = kEntityArmor;
			subEntity = k_ArmorDiamond;
		}
		else if (text[0] == '1' && text[1] == '2')
		{
			entity = kEntityArmor;
			subEntity = k_ArmorTitanium;
		}
		else
		{
			Engine::Report("Pickup Command Help: pickup #");
			Engine::Report("# = 1 -> Health, # = 2 -> Fuel, # = 3 -> Scrap");
			Engine::Report("# = 4 -> Chainsaw, # = 5 -> RocketLauncher, # = 6 -> Blender");
			Engine::Report("# = 7 -> Iron Armor, # = 8 -> Steel Armor, # = 9 -> Gold Armor, # = 10 -> Bronze Armor, # = 11 -> Diamond Armor, # = 12 -> Titanium Armor");
		}

		TheMessageMgr->SendMessageAll(SpawnDropMessage(contIndex, loc, entity, subEntity), true); 
	}
}

void Game::SpawnAICallback(Command *command, const char *text)
{
	if (TheMessageMgr->Server())
	{
		int32 contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
		int range = Math::Random(ThePlayerManager->AISpawnList.size());
		Point3D loc = ThePlayerManager->AISpawnList.at(range);
	
		TheMessageMgr->SendMessageAll(SpawnAIMessage(contIndex, loc));
	}
}

void Game::DisableAICallback(Command *command, const char *text)
{
	if (TheMessageMgr->Server())
	{
		Engine::Report("Disabling AI");
		TheEntityManager->MAX_ENEMIES = 0;
		for (int i = 0; i < ThePlayerManager->EntityList.size(); i++)
		{
			if (ThePlayerManager->EntityList.at(i)->getType() == kEntityEnemy)
			{
				ServerHealthUpdateMessage message(kMessageHealthUpdate, ThePlayerManager->EntityList.at(i)->getID() , 10000, kEntityEnemy, NULL);
				TheMessageMgr->SendMessage(kPlayerServer, message);
			}
		}
	}
}

void Game::EnableAICallback(Command *command, const char *text)
{
	if (TheMessageMgr->Server())
	{
		Engine::Report("Enabling AI");
		TheEntityManager->MAX_ENEMIES = 6;
		
	}
}

void Game::GetAllItemsCallback(Command *command, const char *text)
{
	if (TheMessageMgr->Server())
	{
		Engine::Report("Getting All Items");
		Player * player = TheMessageMgr->GetLocalPlayer();
		GamePlayer * gp = static_cast<GamePlayer *>(player);
		MyPlayerController * cont = static_cast<MyPlayerController *>(gp->GetController());

		EntityWeapon *blender = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_BLENDER_DAMAGE, WEAPON_BLENDER_KNOCKBACK, WEAPON_BLENDER_RANGE, nullptr, k_weaponBlender);
		cont->AddItemToInventory(blender);
		EntityWeapon *rocketLauncher = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_BLENDER_DAMAGE, WEAPON_BLENDER_KNOCKBACK, WEAPON_BLENDER_RANGE, nullptr, k_weaponGun);
		cont->AddItemToInventory(rocketLauncher);
		EntityArmor *armor = new EntityArmor(kEntityArmor, ARMOR_DIAMOND_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorDiamond);
		cont->AddItemToInventory(armor);
		EntityArmor *armor2 = new EntityArmor(kEntityArmor, ARMOR_STEEL_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorSteel);
		cont->AddItemToInventory(armor2);
		EntityArmor *armor3 = new EntityArmor(kEntityArmor, ARMOR_BRONZE_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorBronze);
		cont->AddItemToInventory(armor3);
		EntityArmor *armor4 = new EntityArmor(kEntityArmor, ARMOR_TITANIUM_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorTitanium);
		cont->AddItemToInventory(armor4);
		EntityArmor *armor5 = new EntityArmor(kEntityArmor, ARMOR_TITANIUM_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorIron);
		cont->AddItemToInventory(armor5);
		EntityArmor *armor6 = new EntityArmor(kEntityArmor, ARMOR_TITANIUM_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorGold);
		cont->AddItemToInventory(armor6);
		EntityWeapon *chainSaw = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_CHAINSAW_DAMAGE, WEAPON_CHAINSAW_KNOCKBACK, WEAPON_CHAINSAW_RANGE, nullptr, k_weaponChainsaw);
		cont->AddItemToInventory(chainSaw);
		EntityWeapon *selfDestructGun = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), SELF_DESTRUCT_DAMAGE, SELF_DESTRUCT_DAMAGE, SELF_DESTRUCT_DAMAGE, nullptr, k_weaponSelfDestructGun);
		cont->AddItemToInventory(selfDestructGun);
	}
}