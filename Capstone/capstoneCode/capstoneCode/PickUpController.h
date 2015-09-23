#ifndef PICKUPCONTROLLER_H
#define PICKUPCONTROLLER_H

//Engine Headers
#include "C4Controller.h"
#include "C4Configuration.h"
#include "C4Physics.h"
//My Headers
#include "Game.h"
#include "EntityPowerups.h"

namespace C4
{
	class PickUpController : public RigidBodyController, public EntityPowerups
	{
	public:
		//Constructors
		PickUpController();
		PickUpController(unsigned_int32 _spawntype);
		~PickUpController();

		//Accessor
		unsigned_int32 GetType() const
		{
			return spawntype;
		}

		
		static bool ValidNode(const Node *node);

		// Serialization functions for save/load
		void Pack(Packer& data, unsigned_int32 packFlags) const;
		void Unpack(Unpacker& data, unsigned_int32 unpackFlags);

		// User interface functions for world editor
		int32 GetSettingCount(void) const;
		Setting *GetSetting(int32 index) const;
		void SetSetting(const Setting *setting);

		//Methods
		void Preprocess(void);
		RigidBodyStatus HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody); //override;	
		void ReceiveMessage(const ControllerMessage *message);
		ControllerMessage *ConstructMessage(ControllerMessageType type) const;

	private:
		unsigned_int32 spawntype;
		float			spinRate;             // In radians per millisecond
		float			spinAngle;            // The current angle, in radians
		Transform4D		originalTransform;    // The target's original transform

		PickUpController(const PickUpController& myController); //copy constructor

		Controller *Replicate(void) const; //replicate method
	};

}

#endif

