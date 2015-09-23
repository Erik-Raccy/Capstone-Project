#include "ArmorPickUpController.h"

using namespace C4;

ArmorPickUpController::ArmorPickUpController(unsigned_int32 armorType) : RigidBodyController(kControllerArmorPickUp), EntityArmor(kEntityArmor, 0, Point3D(0, 0, 0), "No Model", (EquipmentType)armorType)
{
	armor = armorType;
}
ArmorPickUpController::ArmorPickUpController() : RigidBodyController(kControllerArmorPickUp), EntityArmor(kEntityArmor, 0, Point3D(0, 0, 0), "No Model", k_ArmorRust)
{
	armor = k_ArmorRust;
}

ArmorPickUpController::~ArmorPickUpController()
{
}

ArmorPickUpController::ArmorPickUpController(const ArmorPickUpController& myController) : RigidBodyController(kControllerArmorPickUp), EntityArmor(kEntityArmor, 0, Point3D(0, 0, 0), "No Model", k_ArmorRust)
{
	armor = k_ArmorRust;
}

Controller *ArmorPickUpController::Replicate(void) const
{
	return (new ArmorPickUpController(*this));
}

bool ArmorPickUpController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

void ArmorPickUpController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);
	data << originalTransform;
}

void ArmorPickUpController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);
	data >> originalTransform;
}

int32 ArmorPickUpController::GetSettingCount(void) const
{
	return(0);
}

Setting *ArmorPickUpController::GetSetting(int32 index) const
{
	return (nullptr);
}

void ArmorPickUpController::SetSetting(const Setting *setting)
{
}

void ArmorPickUpController::Preprocess(void)
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

RigidBodyStatus ArmorPickUpController::HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody)
{
	if (contactBody->GetControllerType() == kControllerPlayer && TheMessageMgr->Server())
	{
		TheMessageMgr->SendMessage(kPlayerServer, ClientPickUpMessage(kArmorPickUpMessage, contactBody->GetControllerIndex(), armor));
		TheMessageMgr->SendMessageAll(PickUpObtainedMessage(GetControllerIndex())); //Deletes Pickup for all players
		return kRigidBodyDestroyed;
	}
	return kRigidBodyUnchanged;
}

void ArmorPickUpController::ReceiveMessage(const ControllerMessage *message)
{
	switch (message->GetControllerMessageType())
	{
	case kPickUpObtainedMessage:
	{
		const PickUpObtainedMessage *m = static_cast<const PickUpObtainedMessage *>(message);
		ThePlayerManager->RemoveEntity(this->GetControllerIndex());
		Node *target = GetTargetNode();
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

ControllerMessage *ArmorPickUpController::ConstructMessage(ControllerMessageType type) const
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

void ArmorPickUpController::Move(void)
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