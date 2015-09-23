#ifndef FUELPICKUPCONTROLLER_H
#define FUELPICKUPCONTROLLER_H

#include "PickUpController.h"
#include "EntityFuel.h"

namespace C4
{

	class FuelPickUpController : public RigidBodyController, public EntityFuel
	{
	public:
		//Constructors
		FuelPickUpController();
		~FuelPickUpController();

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
		void Move(void);
	private:
		float sparkletimer = 0;
		float positionTimer = 0;
		//Variable
		Transform4D originalTransform;

		//Constructor
		FuelPickUpController(const FuelPickUpController& myController);
		Controller *Replicate(void) const;
	};
}

#endif