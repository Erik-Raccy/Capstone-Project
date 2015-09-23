#include "MyMoveAction.h"
using namespace C4;

MovementAction::MovementAction(unsigned_int32 type) : Action(type)
{
	movementType = type;
}


MovementAction::~MovementAction()
{
}

// If a movement button was pressed
void MovementAction::Begin(void) 
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player) //If it is the local player 
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		if (controller)
		{
			//send a movement message to the server
			ClientMovementMessage message(kMessageMovementBegin, movementType);
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}
}

// If a movement button was released
void MovementAction::End(void)
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)  //If it is the local playe
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		if (controller)
		{
			//send a movement message to the server
			ClientMovementMessage message(kMessageMovementEnd, movementType);
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}
}