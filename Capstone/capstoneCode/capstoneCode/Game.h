#ifndef Game_h
#define Game_h

/*
* File:  Game.h
* --------------------------
* This file contains the game singleton class
* which is a child of the application class. it contains
* the application level networking and handles game level
* messages. This class also handles the registration and 
* initialization of all controllers, models, markers,
* commands and database. This class is very important to 
* the game
*/


//Engine headers
#include "C4Application.h" //needed to get started
#include "C4Engine.h" //needed to get started
#include "C4Input.h"
#include "C4Character.h"
#include "C4World.h" // needed by the MyWorld class to create the game world
#include "C4Commands.h" // needed for terminal commands

//My headers
#include "PickUpController.h"
#include "FuelPickUpController.h"
#include "HealthPickUpController.h"
#include "ScrapPickUpController.h"
#include "ArmorPickUpController.h"
#include "WeaponPickUpController.h"
#include "MyCamera.h"
#include "MyWorld.h"
#include "MyPlayerController.h"
#include "MyMoveAction.h"
#include "MyMessages.h"
#include "MyPlayerManager.h"
#include "EntityManager.h"
#include "MyAIController.h"
#include "MyHud.h"
#include "MyAttackAction.h"
#include "MyPauseAction.h"
#include "MySelfDestructAction.h"
#include "MyProjectileController.h"
#include "SparkParticleSystem.h"

#include <map>
#include "DataBaseType.h"

extern "C"
{
	C4MODULEEXPORT C4::Application *ConstructApplication(void);
}

namespace C4
{

	//controller IDs
	enum controlID
	{
		kControllerPlayer = 'play',
		kControllerPickUp = 'pick',
		kControllerFuelPickUp = 'fpck',
		kControllerHealthPickUp = 'hpck',
		kControllerScrapPickUp = 'spck',
		kControllerArmorPickUp = 'apck',
		kControllerWeaponPickUp = 'wpck',
		kControllerAI = 'aico',
		kControllerProjectile = 'proj'
	};

	//input IDs
	enum inputIDro
	{
		kActionForward = 'frwd',
		kActionBackward = 'bkwd',
		kActionLeft = 'left',
		kActionRight = 'rght',
		kActionUp = 'jump',
		kActionPause = 'pist',
		kActionAttack = 'fire',
		kActionSelfDestruct = 'dstr'
	};

	//network IDs
	enum networkID
	{
		kGameProtocol = 0x000B0BA0, //ID for the game
		kGamePort = 12123
	};

	//particle systems
	enum PartileID
	{
		kParticleSystemSpark = 'sprk'
	};

	//Models
	enum modelID
	{
		kModelRobot = 'robo',
		kModelAI = 'aiml',
		kModelChainsaw = 'csaw',
		kModelBlender = 'blen',
		kModelRocketLauncher = 'rola',
		kModelClockLauncher = 'clln',
		kModelClockLauncherPickup = 'cllp',
		kModelCube = 'cube',
		kModelFuelCan = 'fuel',
		kModelToolBox = 'tool',
		kModelIngot = 'ingt',
		kModelBullet = 'bllt',
		kModelBodyArmor = 'bamr',
		kModelBlenderPickup = 'blpk',
		kModelChainsawPickup = 'cspk',
		kModelRocketLauncherPickup = 'rlpk',
		kModelMissle= 'miss',
		kModelClock = 'clck'
	};

	enum markerID
	{
		kMarkerSpawn,
		kMarkerAISpawn,
		kMarkerPathNode,
		kMarkerPickupNode,
		kMarkerWeaponNode
	};

	//prototypes
	class MyPlayerController;
	class PickUpController;
	class FuelPickUpController;
	class HealthPickUpController;
	class ScrapPickUpController;
	class ArmorPickUpController;
	class WeaponPickUpController;
	class MyAIController;
	class MovementAction;
	class MyCamera;
	class MyHud;
	class MyAttackAction;
	class MyPauseAction;
	class SelfDestructAction;
	class SparkParticleSystem;
	class DataBaseType;
	class SoundMessage;

	class Game : public Application, public Singleton<Game>
	{
	public:
		//constructors
		Game(void);
		~Game(void);

		//engine methods
		EngineResult LoadWorld(const char *name) override;
		void UnloadWorld(void) override;
			
		//engine message methods
		Message *ConstructMessage(MessageType type, Decompressor& data) const;
		void ReceiveMessage(Player *from, const NetworkAddress& address, const Message *message);

		//event handler
		void HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param);
		void HandlePlayerEvent(PlayerEvent event, Player *player, const void *param); // player event handler (called by engine)
		void HandleGameEvent(GameEvent event, const void *param);
		
		void HostGame();
		void JoinGame(String<> ipAddress);
		void QuitGame();

		void ServerCallback(Command *command, const char *text); // This method will be executed whenever the user uses the server command.
		void JoinCallback(Command *command, const char *text); // This method will be executed whenever the user uses the join command.
		void NameCallback(Command *command, const char *text); //renames the player
		void PickUpCallback(Command *command, const char *text); //for debugging pickups
		void SpawnAICallback(Command *command, const char *text); //spawn a bot for debugging
		
		void DisableAICallback(Command *command, const char *text);
		void EnableAICallback(Command *command, const char *text);
		void GetAllItemsCallback(Command *command, const char *text);

		static World *ConstructWorld(const char *name, void *data);
		static Player *ConstructPlayer(PlayerKey key, void *data);
		Sound *music;

		//database of players
		std::map<unsigned_int32, DataBaseType> * myDatabase;
	private:
		
		MyHud *myHud;

		//registrations with the engine
		ControllerReg<MyPlayerController>		MyPlayerControllerReg;
		ControllerReg<PickUpController>			PickUpControllerReg;
		ControllerReg<FuelPickUpController>		FuelPickUpControllerReg;
		ControllerReg<HealthPickUpController>	HealthPickUpControllerReg;
		ControllerReg<ScrapPickUpController>	ScrapPickUpControllerReg;
		ControllerReg<ArmorPickUpController>	ArmorPickUpControllerReg;
		ControllerReg<ArmorPickUpController>	WeaponPickUpControllerReg;
		ControllerReg<MyAIController>			MyAIControllerReg;
		DisplayEventHandler						MydisplayEventHandler;

		//Models
		ModelRegistration					robotModelReg;
		ModelRegistration					aiModelReg;
		ModelRegistration					chainsawModelReg;
		ModelRegistration					blenderModelReg;
		ModelRegistration					cubeModelReg;
		ModelRegistration					bulletModelReg;
		ModelRegistration					fuelCanModelReg;
		ModelRegistration					toolBoxModelReg;
		ModelRegistration					ingotModelReg;
		ModelRegistration					bodyArmorModelReg;
		ModelRegistration					blenderPickupModelReg;
		ModelRegistration					chainsawPickupModelReg;
		ModelRegistration					rocketLauncherModelReg;
		ModelRegistration					rocketLauncherPickupModelReg;
		ModelRegistration					clockModelReg;
		ModelRegistration					clockLauncherModelReg;
		ModelRegistration					clockLauncherPickupModelReg;

		//Particles
		ParticleSystemReg<SparkParticleSystem>	sparkParticleSystemReg;

		//Locations
		LocatorRegistration				    spawnReg;
		LocatorRegistration				    aispawnReg;
		LocatorRegistration				    pathnodeReg;
		LocatorRegistration				    pickupReg;

		static void HandleDisplayEvent(const DisplayEventData *eventData, void *cookie);
		
		// actions & inputs
		MovementAction						*forwardAction;
		MovementAction						*backwardAction;
		MovementAction						*leftAction;
		MovementAction						*rightAction;
		MovementAction						*upAction;
		MyAttackAction						*attackAction;
		MyPauseAction						*pauseAction;
		SelfDestructAction					*selfDestructAction;

		//console commands + observers
		CommandObserver<Game> *serverObserver;
		CommandObserver<Game> *nameObserver;
		CommandObserver<Game> *joinObserver;
		CommandObserver<Game> *spawnPickUpObserver;
		CommandObserver<Game> *spawnAIObserver;
		
		CommandObserver<Game> *DisableAIObserver;
		CommandObserver<Game> *EnableAIObserver;
		CommandObserver<Game> *GetAllItemsObserver;
		
		Command *serverCommand; 
		Command *nameCommand;
		Command *joinCommand;
		Command *spawnPickUpCommand;
		Command *spawnAICommand;
		
		Command *disableAICommand;
		Command *enableAICommand;
		Command *GetAllItemsCommand;
	};

	extern Game *TheGame; //singleton global accessor
}

#endif