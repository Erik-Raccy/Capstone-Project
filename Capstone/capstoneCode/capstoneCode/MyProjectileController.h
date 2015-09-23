#ifndef MYPROJECTILECONTROLLER_H
#define MYPROJECTILECONTROLLER_H

/*
* File:  MyProjectileController.h
* --------------------------------
* This file contains the projectilecontroller 
* class. This class contains the behavious for the
* bullets fired from the projectile weapons
*/

//Engine Headers
#include "C4Controller.h"
#include "C4Configuration.h"
//My Headers
#include "Game.h"
#include "MyMessages.h"
#include "EntityBullet.h"

namespace C4
{
	class MyProjectileController : public RigidBodyController, public EntityBullet
	{
	public:

		//Constructors
		MyProjectileController(unsigned_int32 _shotType, Vector3D _velocity);
		MyProjectileController();
		~MyProjectileController();

		static bool ValidNode(const Node *node);

		// Serialization functions for save/load
		void Pack(Packer& data, unsigned_int32 packFlags) const;
		void Unpack(Unpacker& data, unsigned_int32 unpackFlags);

		// User interface functions for world editor
		int32 GetSettingCount(void) const;
		Setting *GetSetting(int32 index) const;
		void SetSetting(const Setting *setting);

		void Preprocess(void);
		void Move(void);// The function that moves the target node

		RigidBodyStatus HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody);
		RigidBodyStatus HandleNewGeometryContact(const RigidBodyContact *contact);

		ControllerMessage *ConstructMessage(ControllerMessageType type) const;
		void ReceiveMessage(const ControllerMessage *message);

		Vector3D GetDirection()
		{
			return velocity;
		}

		unsigned_int32 GetProjType()
		{
			return projectileType;
		}

	private:

		unsigned_int32 projectileType;
		Vector3D velocity;
		Transform4D   originalTransform;    // The target's original transform
		int32 killTimer;

		MyProjectileController(const MyProjectileController& myController); //copy constructor

		RigidBodyController *Replicate(void) const; //replicate method

	};

}

#endif

