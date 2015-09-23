#include "MyPauseAction.h"
#include "InventoryWindow.h"
using namespace C4;

MyPauseAction::MyPauseAction(unsigned_int32 type) : Action(type)
{
}

MyPauseAction::~MyPauseAction()
{
	delete TheInventoryWindow;
}

// When the '1' button is pressed, open up inventory
void MyPauseAction::Begin(void)
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player) // If it is the local player
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		if (controller)
		{
			controller->Crouching = true;
			controller->newAnimation = kCrouchAnimation;
			TheInventoryWindow->Open(); // Open the inventory

			//Play the open inventory sound locally 
			Point3D Loc = controller->GetTargetNode()->GetWorldPosition();
			SoundMessage sMessage(kMessageSound, kZipperSound, 100.0f, Loc);
			TheMessageMgr->SendMessage(controller->getPlayerKey(), sMessage);

			//Add all the equipment the player currently has to their inventory
			for (int i = 0; i < controller->InventoryEquipment.size(); i++) 
			{
				TheInventoryWindow->AddEquipmentToInventory(controller->InventoryEquipment[i]);
			}
			// Add the images of the equipment to the window
			TheInventoryWindow->AddEquipmentToWindow();
		}
	}
}

void MyPauseAction::End(void)
{

}