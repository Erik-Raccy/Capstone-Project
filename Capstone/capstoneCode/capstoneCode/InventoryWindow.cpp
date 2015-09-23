#include "Game.h"
#include "C4Input.h"
#include "InventoryWindow.h"

using namespace C4;

InventoryWindow *C4::TheInventoryWindow = nullptr;

InventoryWindow::InventoryWindow() : Window("panel/Inventory"), Singleton<InventoryWindow>(TheInventoryWindow)
{
	
}

InventoryWindow::~InventoryWindow()
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player) // If local player
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		if (controller)
		{
			controller->mainFrameInterpolator->SetMode(kInterpolatorBackward);
		}
	}

	this->RemoveSubnode(EquipButton);
	delete EquipButton;
	this->RemoveSubnode(DropButton);
	delete DropButton;
	this->RemoveSubnode(QuitButton);
	delete QuitButton;
	this->RemoveSubnode(RemoveWeapon);
	delete RemoveWeapon;
	this->RemoveSubnode(RemoveArmor);
	delete RemoveArmor;

	//Remove all the images of the equipment from the inventory
	for (int32 i = 0; i < InventoryTable->GetTableItemCount(); i++)
	{
		delete InventoryTable->GetTableItem(i);
	}
	this->RemoveSubnode(InventoryTable);
	delete InventoryTable;
	this->RemoveSubnode(WeaponImage);
	delete WeaponImage;
	this->RemoveSubnode(ArmorImage);
	delete ArmorImage;
	this->RemoveSubnode(Background);
	delete Background;
}

void InventoryWindow::Preprocess()
{
	Window::Preprocess();

	//Get the widgets from the panel
	EquipButton = static_cast<PushButtonWidget *>(this->FindWidget("EquipButton"));
	DropButton = static_cast<PushButtonWidget *>(this->FindWidget("DropButton"));
	QuitButton = static_cast<PushButtonWidget *>(this->FindWidget("QuitButton"));
	RemoveWeapon = static_cast<PushButtonWidget *>(this->FindWidget("RemoveWeapon"));
	RemoveArmor = static_cast<PushButtonWidget *>(this->FindWidget("RemoveArmor"));

	InventoryTable = static_cast<TableWidget *> (this->FindWidget("InventoryWindow"));
	WeaponImage = static_cast<ImageWidget *> (this->FindWidget("WeaponImage"));
	Background = static_cast<ImageWidget *> (this->FindWidget("Background"));
	ArmorImage = static_cast<ImageWidget *> (this->FindWidget("ArmorImage"));

	InventoryTable->SetColumnCount(5);
	InventoryTable->SetCellSize(Vector2D(55, 55));

	// put window in center of screen
	TheInventoryWindow->SetWidgetPosition(Point3D(TheDisplayMgr->GetDisplayWidth() / 2.0 -256, TheDisplayMgr->GetDisplayHeight() / 2.0 -256, 0));

	// Checks to see if the player initially has anything equipped, if so add it to their character in the inventory
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		if (controller)
		{
			if (controller->getArmor()->GetArmorType() == k_ArmorRust) //If they do not have armor equipped
			{
				ArmorEquipped = false;
			}

			else //If they do have armor equipped
			{
				ArmorEquipped = true;
				ArmorImage->SetTexture(0, GetInventoryTexture(controller->getArmor()->GetArmorType()));
			}

			if (controller->getWeapon()->GetWeaponType() == k_weaponNoWeapon)  //If they do not have a weapon equipped
			{
				WeaponEquipped = false;
			}

			else  //If they do have armor equipped
			{
				WeaponEquipped = true;
				WeaponImage->SetTexture(0, GetInventoryTexture((controller->getWeapon()->GetWeaponType())));
			}
		}

	}
}

//Handle when the player uses one of the widgets
void InventoryWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData)
{
	if (eventData->eventType == kEventWidgetActivate)
	{
		// If the user presses the equipped button AND has something selected
		if (widget == EquipButton && InventoryTable->GetSelectedTableItem(0)!=NULL)
		{
			//Get the index of the item selected
			int32 index = InventoryTable->GetFirstSelectedIndex();

			// If the equipment is Armor
			if (equipmentList[index]->getType() == kEntityArmor)
			{
					Player *player = TheMessageMgr->GetLocalPlayer();
					if (player) //If local player
					{
						MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
						if (controller)
						{
							// changes the armor image in the inventory screen
							ArmorImage->SetTexture(0, GetInventoryTexture(equipmentList[index]->getEquipmentName()));
							ArmorEquipped = true;

							//Player equip sound locally
							Point3D Loc = controller->GetTargetNode()->GetWorldPosition();
							SoundMessage sMessage(kMessageSound, kEquipSound, 100.0f, Loc);
							TheMessageMgr->SendMessage(controller->getPlayerKey(), sMessage);

							//Send a message to the server to change players armor
							ChangeArmorMessage message(kMessageUpdateArmor, equipmentList[index]->getEquipmentName(), controller->GetControllerIndex());
							TheMessageMgr->SendMessage(kPlayerServer, message);			
						}
					}
			}

			// Equipment is a weapon
			else 
			{ 
				Player *player = TheMessageMgr->GetLocalPlayer();
				if (player) //If local player
				{
					MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
					if (controller)
					{
						// Set the weapon image in the inventory screen
						WeaponImage->SetTexture(0, GetInventoryTexture(equipmentList[index]->getEquipmentName()));
						WeaponEquipped = true;

						//Player equip sound locally
						Point3D Loc = controller->GetTargetNode()->GetWorldPosition();
						SoundMessage sMessage(kMessageSound, kEquipSound, 100.0f, Loc);
						TheMessageMgr->SendMessage(controller->getPlayerKey(), sMessage);

						//Send a message to the server to change players weapon
						UpdateWeaponMessage message(kMessageUpdateWeapon, equipmentList[index]->getEquipmentName(), controller->GetControllerIndex());
						TheMessageMgr->SendMessage(kPlayerServer, message);
					}
				}
			}
		}

		// If the user presses the drop button AND has something selected
		else if (widget == DropButton && InventoryTable->GetSelectedTableItem(0) != NULL)
		{
			//Get the index of the item selected
			int32 index = InventoryTable->GetFirstSelectedIndex();

			// If the equipment is Armor
			if (equipmentList[index]->getType() == kEntityArmor)
			{
				Player *player = TheMessageMgr->GetLocalPlayer();
				if (player) // If local player
				{
					MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
					if (controller)
					{
						//Get the players currently equipped armor
						unsigned_int32 armorEquipped = controller->getArmor()->GetArmorType();

						//Check to see if the armor they dropped is the one they are currently using
						if (armorEquipped == equipmentList[index]->getEquipmentName())
						{
							// If so send message to server to update the armor of the player to the default rust armor
							ChangeArmorMessage message(kMessageUpdateArmor, k_ArmorRust, controller->GetControllerIndex());
							TheMessageMgr->SendMessage(kPlayerServer, message);

							//Set the armor in the inventory to the default armor
							ArmorEquipped = false;
							ArmorImage->SetTexture(0, "textures/aRustArmor");		
							EntityArmor *defaultArmor = new EntityArmor(kEntityArmor, ARMOR_RUST_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorRust);
							controller->setArmor(defaultArmor);
						}
						//Remove the equipment from the player's inventory locally
						controller->RemoveItemToInventory(equipmentList[index]->getEquipmentName());
						Widget *removedWidget = InventoryTable->GetSelectedTableItem(0);
						InventoryTable->RemoveTableItem(removedWidget);
						this->RemoveSubnode(removedWidget);
						delete removedWidget;
						

						Engine::Report(Text::IntegerToString(equipmentList[index]->getEquipmentName()));
						//Remove the equipment from the player's inventory on the server
						RemoveArmorAndWeaponsFromInventoryMessage msg(kRemoveArmorAndWeaponFromInventoryMessage, controller->GetControllerIndex(), equipmentList[index]->getEquipmentName());
						TheMessageMgr->SendMessage(kPlayerServer, msg);
						

						equipmentList.erase(equipmentList.begin() + index);
						//Play the drop item sound locally
						Point3D Loc = controller->GetTargetNode()->GetWorldPosition();
						SoundMessage sMessage(kMessageSound, kDropItemSound, 100.0f, Loc);
						TheMessageMgr->SendMessage(controller->getPlayerKey(), sMessage);		
					}
				}
			}
			//If the equipment is a weapon
			else
			{
				Player *player = TheMessageMgr->GetLocalPlayer();
				if (player) //If local player
				{
					MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
					if (controller)
					{
						//Get the players currently equipped weapon
						unsigned_int32 weaponEquipped = controller->getWeapon()->GetWeaponType();

						//Check to see if the weapon they dropped is the one they are currently using
						if (weaponEquipped == equipmentList[index]->getEquipmentName())
						{
							// If so send message to server to update the weapon of the player to the default weapon
							UpdateWeaponMessage message(kMessageUpdateWeapon, k_weaponNoWeapon, controller->GetControllerIndex());
							TheMessageMgr->SendMessage(kPlayerServer, message);

							//set the weapon in the inventory to the default weapon (fists)
							WeaponEquipped = false;
							WeaponImage->SetTexture(0, "textures/wNoWeapon"); 
							EntityWeapon *defaultWeapon = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_NO_WEAPON_DAMAGE, WEAPON_NO_WEAPON_KNOCKBACK, WEAPON_NO_WEAPON_RANGE, nullptr, k_weaponNoWeapon);
							controller->setWeapon(defaultWeapon);
						}
						//Remove the equipment from the player's inventory locally
						controller->RemoveItemToInventory(equipmentList[index]->getEquipmentName());
						Widget *removedWidget = InventoryTable->GetSelectedTableItem(0);
						InventoryTable->RemoveTableItem(removedWidget);
						this->RemoveSubnode(removedWidget);
						delete removedWidget;

						//Remove the equipment from the player's inventory on the server
						RemoveArmorAndWeaponsFromInventoryMessage msg(kRemoveArmorAndWeaponFromInventoryMessage, controller->GetControllerIndex(), equipmentList[index]->getEquipmentName());
						TheMessageMgr->SendMessage(kPlayerServer, msg);


						equipmentList.erase(equipmentList.begin() + index);
						//Play the drop item sound locally
						Point3D Loc = controller->GetTargetNode()->GetWorldPosition();
						SoundMessage sMessage(kMessageSound, kDropItemSound, 100.0f, Loc);
						TheMessageMgr->SendMessage(controller->getPlayerKey(), sMessage);
					}
				}
			}
		}

		// If they press the remove armor button, and they actually have armor to remove -> then remove it
		else if (widget == RemoveArmor && ArmorEquipped==true)
		{
			ArmorEquipped = false;
			ArmorImage->SetTexture(0, "textures/aRustArmor"); //set the armor in the inventory screen to the default rust armor			

			Player *player = TheMessageMgr->GetLocalPlayer();
			if (player) // If local player
			{
				MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
				if (controller)
				{
					// set the player's armor to the default rust armor
					EntityArmor *defaultArmor = new EntityArmor(kEntityArmor, ARMOR_RUST_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorRust);
					controller->setArmor(defaultArmor);

					//Send message to server to update the armor of the player to the default rust armor
					ChangeArmorMessage message(kMessageUpdateArmor, k_ArmorRust, controller->GetControllerIndex());
					TheMessageMgr->SendMessage(kPlayerServer, message);
				}
			}
		}

		// If they press the remove weapon button, and they actually have a weapon to remove -> then remove it
		else if (widget == RemoveWeapon &&  WeaponEquipped == true)
		{
			WeaponEquipped = false;
			WeaponImage->SetTexture(0, "textures/wNoWeapon"); //set the weapon in the inventory to the default weapon (fists)	

			Player *player = TheMessageMgr->GetLocalPlayer();
			if (player) // If local player
			{
				MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
				if (controller)
				{
					// set the player's weapon to the default weapon (fists)
					EntityWeapon *defaultWeapon = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_NO_WEAPON_DAMAGE, WEAPON_NO_WEAPON_KNOCKBACK, WEAPON_NO_WEAPON_RANGE, nullptr, k_weaponNoWeapon);
					controller->setWeapon(defaultWeapon);

					//Send message to server to update the weapon of the player to the default weapon
					UpdateWeaponMessage message(kMessageUpdateWeapon, k_weaponNoWeapon, controller->GetControllerIndex());
					TheMessageMgr->SendMessage(kPlayerServer, message);
				}
			}
		}
		
		// if the user presses the quit button, close the inventory screen
		else if (widget == QuitButton)
		{
			this->Close();
		}
	}
}
// Called when player presses the '1' button
void InventoryWindow::Open()
{
	if (TheInventoryWindow)
	{
		TheInterfaceMgr->SetActiveWindow(TheInventoryWindow);
	}
	else
	{
		TheInterfaceMgr->AddWidget(new InventoryWindow);
	}
}

void InventoryWindow::GraphicsSettingsComplete(GraphicsSettingsWindow *window, void *cookie)
{
	Open();
}

//Add the equipment to the vector
void InventoryWindow::AddEquipmentToInventory(EntityEquippable *equipment)
{
	equipmentList.push_back(equipment);
}

//Add the elements of the vector to the inventory window
void InventoryWindow::AddEquipmentToWindow()
{
	for (int i = 0; i < equipmentList.size(); i++)
	{
		char *textureName = GetInventoryTexture(equipmentList[i]->getEquipmentName());
		if (textureName)
		{
			ImageWidget *tempImage = new ImageWidget(Vector2D(50, 50), textureName);
			InventoryTable->AppendTableItem(tempImage);
		}
	}
}

// Get the materials for the robot
char* InventoryWindow::GetRobotTexture(unsigned int textureNumber)
{
	switch (textureNumber)
	{
	case k_ArmorIron:
		return "textures/ironMaterial";
	case k_ArmorBronze:
		return "textures/bronzeMaterial";
	case k_ArmorSteel:
		return "textures/steelMaterial";
	case k_ArmorGold:
		return "textures/goldMaterial";
	case k_ArmorDiamond:
		return "textures/diamondMaterial";
	case k_ArmorTitanium:
		return "textures/titaniumMaterial";
	}
	return "textures/rustMaterial";
}

//get the models for the weapons
unsigned_int32 InventoryWindow::GetRobotWeapon(unsigned int weaponNumber)
{
	switch (weaponNumber)
	{
	case k_weaponBlender:
		return kModelBlender;
	case k_weaponChainsaw:
		return kModelChainsaw;
	case k_weaponGun:
		return kModelRocketLauncher;
	case k_weaponSelfDestructGun:
		return kModelClockLauncher;
	}
	return NULL;
}

//Get the textures for the inventory
char* InventoryWindow::GetInventoryTexture(unsigned_int32 textureNumber)
{
	switch (textureNumber)
	{
	//weapons
	case k_weaponBlender:
		return "textures/wBlenderWeapon";
	case k_weaponChainsaw:
		return "textures/wChainsawWeapon";
	case k_weaponTableSaw:
		return "textures/wTableSawWeapon";
	case k_weaponGun:
		return "textures/wRocketLauncherWeapon";
	case k_weaponSelfDestructGun:
		return "textures/selfDestructionGun";

	//Armor
	case k_ArmorIron:
		return "textures/aIronArmor";
	case k_ArmorBronze:
		return "textures/aBronzeArmor";
	case k_ArmorSteel:
		return "textures/aSteelArmor";
	case k_ArmorGold:
		return "textures/aGoldArmor";
	case k_ArmorDiamond:
		return "textures/aDiamondArmor";
	case k_ArmorTitanium:
		return "textures/aTitaniumArmor";

	}
	return nullptr;

}