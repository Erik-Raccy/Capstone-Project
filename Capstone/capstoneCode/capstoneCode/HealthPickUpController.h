#ifndef HEALTHPICKUPCONTROLLER_H
#define HEALTHPICKUPCONTROLLER_H

#include "PickUpController.h"
#include "EntityHealth.h"

namespace C4
{

	class HealthPickUpController : public RigidBodyController, public EntityHealth
	{
	public:

		//Constructors
		HealthPickUpController();
		~HealthPickUpController();

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

		//Variables
		float sparkletimer = 0;
		float positionTimer = 0;
		Transform4D originalTransform;

		//Constructor
		HealthPickUpController(const HealthPickUpController& myController);
		Controller *Replicate(void) const;
	};
}

#endif