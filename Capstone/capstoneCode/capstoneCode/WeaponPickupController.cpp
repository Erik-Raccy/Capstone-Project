#include "WeaponPickUpController.h"

using namespace C4;

WeaponPickUpController::WeaponPickUpController(unsigned_int32 weaponType) : RigidBodyController(kControllerWeaponPickUp), EntityWeapon(kEntityWeapon, Point3D(0, 0, 0), 0,0,0,"No Model", (EquipmentType)weaponType)
{
	weapon =weaponType;
}
WeaponPickUpController::WeaponPickUpController() : RigidBodyController(kControllerArmorPickUp), EntityWeapon(kEntityWeapon, Point3D(0, 0, 0), 0, 0, 0, "No Model", k_weaponNoWeapon)
{
	weapon = k_weaponNoWeapon;
}

WeaponPickUpController::~WeaponPickUpController()
{
}

WeaponPickUpController::WeaponPickUpController(const WeaponPickUpController& myController) : RigidBodyController(kControllerWeaponPickUp), EntityWeapon(kEntityWeapon, Point3D(0, 0, 0), 0, 0, 0, "No Model", k_weaponNoWeapon)
{
	weapon = k_weaponNoWeapon;
}

Controller *WeaponPickUpController::Replicate(void) const
{
	return (new WeaponPickUpController(*this));
}

bool WeaponPickUpController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

void WeaponPickUpController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);
	data << originalTransform;
}

void WeaponPickUpController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);
	data >> originalTransform;
}

int32 WeaponPickUpController::GetSettingCount(void) const
{
	return(0);
}

Setting *WeaponPickUpController::GetSetting(int32 index) const
{
	return (nullptr);
}

void WeaponPickUpController::SetSetting(const Setting *setting)
{
}

void WeaponPickUpController::Preprocess(void)
{
	RigidBodyController::Preprocess();
	// Grab the original transform of the target node
	const Node *target = GetTargetNode();
	originalTransform = target->GetNodeTransform();

	SetRigidBodyFlags(kRigidBodyKeepAwake | kRigidBodyFixedOrientation);

	// Set the kGeometryDynamic flag for any geometry nodes
	const Node *node = target;
	do
	{
		if (node->GetNodeType() == kNodeGeometry)
		{
			// Node is a geometry, so grab its object
			GeometryObject *object = static_cast<const Geometry *>(node)->GetObject();

			// Set the kGeometryDynamic flag
			object->SetGeometryFlags(object->GetGeometryFlags() | kGeometryDynamic);
		}

		// Iterate through entire subtree
		node = target->GetNextNode(node);
	} while (node);
}

RigidBodyStatus WeaponPickUpController::HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody)
{
	if (contactBody->GetControllerType() == kControllerPlayer && TheMessageMgr->Server())
	{

		TheMessageMgr->SendMessage(kPlayerServer, ClientPickUpMessage(kWeaponPickUpMessage, contactBody->GetControllerIndex(), weapon));
		TheMessageMgr->SendMessageAll(PickUpObtainedMessage(GetControllerIndex())); //Deletes Pickup for all players
		return kRigidBodyDestroyed;
	}
	return kRigidBodyUnchanged;
}

void WeaponPickUpController::ReceiveMessage(const ControllerMessage *message)
{
	switch (message->GetControllerMessageType())
	{
	case kPickUpObtainedMessage:
	{
								   const PickUpObtainedMessage *m = static_cast<const PickUpObtainedMessage *>(message);
								   Node *target = GetTargetNode();
								   ThePlayerManager->RemoveEntity(this->GetControllerIndex());
								   delete target;
								   break;
	}
	case kNodeMessageState:
	{
							  const PlayerStateMessage *msg = static_cast<const PlayerStateMessage *>(message);
							  GetTargetNode()->SetNodePosition(msg->GetPosition());
							  break;
	}
	}
}

ControllerMessage *WeaponPickUpController::ConstructMessage(ControllerMessageType type) const
{
	switch (type)
	{
	case kPickUpObtainedMessage:
		return (new PickUpObtainedMessage(GetControllerIndex()));
	case kNodeMessageState:
		return (new PlayerStateMessage(GetControllerIndex()));
	}
	return (RigidBodyController::ConstructMessage(type));
}


void WeaponPickUpController::Move(void)
{
	sparkletimer += TheTimeMgr->GetDeltaTime();
	if (sparkletimer >= 200)
	{
		sparkletimer = 0;
		SparkParticleSystem *spark = new SparkParticleSystem(0.05F);
		spark->SetNodePosition(GetTargetNode()->GetNodePosition());
		TheWorldMgr->GetWorld()->AddNewNode(spark);

	}
	if (TheMessageMgr->Server())
	{
		positionTimer += TheTimeMgr->GetDeltaTime();
		if (positionTimer >= 1000)
		{
			positionTimer = 0;
			PlayerStateMessage msg(GetControllerIndex(), GetTargetNode()->GetNodePosition());
			TheMessageMgr->SendMessageAll(msg, false);
		}

	}
}