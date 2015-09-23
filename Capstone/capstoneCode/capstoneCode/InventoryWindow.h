#pragma once

/*
* File:  InventoryWindow.h
* --------------------------
* This file contains the inventory window
* class. This class is responsible for drawing
* and updating the various widgets displaying the 
* players inventory and updating them accordingly.
* it also provides interaction for equipping and 
* dropping weapons and armor.
*/

#include "C4Interface.h"

using namespace C4;

namespace C4
{
	class GraphicsSettingsWindow;

	class InventoryWindow : public Window, public Singleton<InventoryWindow>
	{
	public:
		InventoryWindow();
		~InventoryWindow();

		static void Open();

		void Preprocess();
		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData);
		void AddEquipmentToInventory(EntityEquippable * equipment);
		void AddEquipmentToWindow();
		static void GraphicsSettingsComplete(GraphicsSettingsWindow *window, void *cookie);

		char* GetInventoryTexture(unsigned_int32 textureNum);
		char* GetRobotTexture(unsigned_int32 textureNum);
		unsigned_int32 GetRobotWeapon(unsigned_int32 textureNum);

	private:

		//Widgets
		PushButtonWidget	*EquipButton;
		PushButtonWidget	*DropButton;
		PushButtonWidget	*QuitButton;
		PushButtonWidget	*RemoveWeapon;
		PushButtonWidget	*RemoveArmor;

		TableWidget			*InventoryTable;
		ImageWidget			*Background;
		ImageWidget			*WeaponImage;
		ImageWidget			*ArmorImage;

		bool				ArmorEquipped; //Does the player currently have armor equipped
		bool				WeaponEquipped;//Does the player currently have a weapon equipped 

		//The players inventrory
		std::vector<EntityEquippable *> equipmentList;
	};
	extern InventoryWindow *TheInventoryWindow;
}