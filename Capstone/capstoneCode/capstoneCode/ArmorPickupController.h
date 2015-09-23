#ifndef ARMORPICKUPCONTROLLER_H
#define ARMORPICKUPCONTROLLER_H

#include "PickUpController.h"
#include "EntityArmor.h"

namespace C4
{

	class ArmorPickUpController : public RigidBodyController, public EntityArmor
	{
	public:
		//Constructors
		ArmorPickUpController(unsigned_int32 armorType);
		ArmorPickUpController();
		~ArmorPickUpController();

		//Methods
		static bool ValidNode(const Node *node);
		void Pack(Packer& data, unsigned_int32 packFlags) const;
		void Unpack(Unpacker& data, unsigned_int32 unpackFlags);
		int32 GetSettingCount(void) const;
		Setting *GetSetting(int32 index) const;
		void SetSetting(const Setting *setting);
		void Preprocess(void);
		RigidBodyStatus HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody); //override;
		void ReceiveMessage(const ControllerMessage *message);
		ControllerMessage *ConstructMessage(ControllerMessageType type) const;

		void Move(void);// The function that moves the target node

	private:
		//Variables
		Transform4D originalTransform;
		unsigned_int32 armor;
		float sparkletimer = 0;
		float positionTimer = 0;
		//Constructor
		Controller *Replicate(void) const;
		ArmorPickUpController(const ArmorPickUpController& myController);


	};
}

#endif