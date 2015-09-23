#include "MySelfDestructAction.h"
using namespace C4;

SelfDestructAction::SelfDestructAction(unsigned_int32 type) : Action(type)
{
	destructType = type;
}


SelfDestructAction::~SelfDestructAction()
{

}

// If a movement button was pressed
void SelfDestructAction::Begin(void)
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		controller->SetSelfDestruct();
	}
}

// If a movement button was released
void SelfDestructAction::End(void)
{
	
}