#ifndef MyMessages_h
#define MyMessages_h

/*
* File:  MyMessages.h
* --------------------------
* This file is an empty header including 
* all the various message classes used in 
* game. Used as a placeholder for including 
* the individual messages
*/

//Engine 
#include "C4Messages.h" // needed for sending messages between classes + multiplayer
#include "C4Character.h"
#include "C4Physics.h"
#include "C4World.h"

//My headers
// all the message classes
#include "GamePlayer.h"
#include "SpawnMessage.h"
#include "ClientMovementMessage.h"
#include "ClientOrientationMessage.h"
#include "RequestMessage.h"
#include "RequestRenameMessage.h" // also has NameChangeMessage class
#include "SpawnMessage.h"
#include "PlayerStateMessage.h"
#include "PlayerDestroyMessage.h"
#include "PlayerMovementMessage.h" // also has CharacterStateMessage class
#include "PlayerOrientationMessage.h"
#include "PlayerInformationMessage.h"
#include "PickUpObtainedMessage.h"
#include "ClientPickUpMessage.h"
#include "AttackMessage.h"
#include "ServerHealthUpdateMessage.h"
#include "ChangeAIStateMessage.h"
#include "SpawnParticleMessage.h"
#include "ChangeArmorMessage.h"
#include "ChangeArmorMessageAll.h"
#include "UpdateWeaponMessage.h"
#include "UpdateWeaponMessageAll.h"
#include "EntityRemoverMessage.h"
#include "AddArmorToInventoryMessage.h"
#include "AddWeaponToInventoryMessage.h"
#include "AddArmorAndWeaponToPlayersMessage.h"
#include "SoundMessage.h"
#include "NewProjectileRequestMessage.h"
#include "SpawnProjectileMessage.h"
#include "DestroyProjectileMessage.h"
#include "RemoveArmorAndWeaponsFromInventoryMessage.h"
#include "PlayerKillRequestMessage.h"
#include "PlayerChangeAnimationStateMessage.h"

namespace C4 {

	enum messageID
	{
		kMessageServerInfo = kMessageBaseCount,
		kMessageNameChangeRequestMessage, 	// This is the identifier for our name change request message.
		kMessageNameChangeMessage, // This is the identifier for our name change confirmation message
		kMessageSpawn,
		kMessageRespawn,
		kMessageRequest,
		kMessageMovementBegin,
		kMessageMovementEnd,
		kMessageOrientation,
		kMessageInformation,
		kPlayerMessageBeginMovement,
		kPlayerMessageEndMovement,
		kNodeMessageState,
		kPlayerMessageDestroy,
		kPlayerMessageOrientation,
		kAIMessageState,
		kAISpawnMessage,
		kEntityRemoveMessage,
		kNewProjectileRequestMessage,
		kSpawnProjectileMessage,
		kDestroyProjectileMessage,
		kChangeStateMessage,
		kSpawnParticleMessage,
		kPlayerKillRequestMessage,

		//pickup messages
		kPickUpObtainedMessage,
		kDefaultPickUpMessage,
		kFuelPickUpMessage,
		kHealthPickUpMessage,
		kScrapPickUpMessage,
		kArmorPickUpMessage,
		kWeaponPickUpMessage,
		kPlayerChangeAnimationStateMessage,

		kPlayerInformationMessage, //Message that passes health,fuel, and scraps
		kMessageDropSpawn, // debuging code
		kMessageHealthUpdate,
		kMessageAttack,
		kMessageUpdateArmor,
		kMessageUpdateArmorAll,
		kMessageUpdateWeapon,
		kMessageUpdateWeaponAll,
		kMessageAddArmorToInventory,
		kMessageAddWeaponToInventory,
		kAddArmorAndWeaponMessage,
		kMessageSound,
		kRemoveArmorAndWeaponFromInventoryMessage,
		kMessagePause
	};
		
};
#endif
