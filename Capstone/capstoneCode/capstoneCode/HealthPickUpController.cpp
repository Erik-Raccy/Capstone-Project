#include "HealthPickUpController.h"

using namespace C4;

HealthPickUpController::HealthPickUpController() : RigidBodyController(kControllerFuelPickUp), EntityHealth(kEntityHealth, 0, Point3D(0, 0, 0), "No Model")
{
}

HealthPickUpController::~HealthPickUpController()
{
}

HealthPickUpController::HealthPickUpController(const HealthPickUpController& myController) : RigidBodyController(kControllerFuelPickUp), EntityHealth(kEntityHealth, 0, Point3D(0, 0, 0), "No Model")
{
}

Controller *HealthPickUpController::Replicate(void) const
{
	return (new HealthPickUpController(*this));
}

bool HealthPickUpController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

void HealthPickUpController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);
	data << originalTransform;
}

void HealthPickUpController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);
	data >> originalTransform;
}

int32 HealthPickUpController::GetSettingCount(void) const
{
	return(0);
}

Setting *HealthPickUpController::GetSetting(int32 index) const
{
	return (nullptr);
}

void HealthPickUpController::SetSetting(const Setting *setting)
{
}

void HealthPickUpController::Preprocess(void)
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

RigidBodyStatus HealthPickUpController::HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody)
{
	if (contactBody->GetControllerType() == kControllerPlayer && TheMessageMgr->Server())
	{
		TheMessageMgr->SendMessage(kPlayerServer, ClientPickUpMessage(kHealthPickUpMessage, contactBody->GetControllerIndex(),0));
		TheMessageMgr->SendMessageAll(PickUpObtainedMessage(GetControllerIndex())); //Deletes Pickup for all players
		return kRigidBodyDestroyed;
	}

	return kRigidBodyUnchanged;
}

void HealthPickUpController::ReceiveMessage(const ControllerMessage *message)
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

ControllerMessage *HealthPickUpController::ConstructMessage(ControllerMessageType type) const
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


void HealthPickUpController::Move(void)
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