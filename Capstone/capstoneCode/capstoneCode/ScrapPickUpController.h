#ifndef SCRAPPICKUPCONTROLLER_H
#define SCRAPPICKUPCONTROLLER_H

#include "PickUpController.h"
#include "EntityScraps.h"

namespace C4
{
	class ScrapPickUpController : public RigidBodyController, public EntityScraps
	{
	public:
		//Constructors
		ScrapPickUpController();
		~ScrapPickUpController();

		static bool ValidNode(const Node *node);

		// Serialization functions for save/load
		void Pack(Packer& data, unsigned_int32 packFlags) const;
		void Unpack(Unpacker& data, unsigned_int32 unpackFlags);

		// User interface functions for world editor
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
		Transform4D		originalTransform;    // The target's original transform
		ScrapPickUpController(const ScrapPickUpController& myController); //copy constructor
		Controller *Replicate(void) const; //replicate method
	};

}

#endif
