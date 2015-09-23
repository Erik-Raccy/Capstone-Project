#include "FuelPickUpController.h"

using namespace C4;

FuelPickUpController::FuelPickUpController() : RigidBodyController(kControllerFuelPickUp), EntityFuel(kEntityFuel, 0, Point3D(0, 0, 0), "No Model")
{
}

FuelPickUpController::~FuelPickUpController()
{
}

FuelPickUpController::FuelPickUpController(const FuelPickUpController& myController) : RigidBodyController(kControllerFuelPickUp), EntityFuel(kEntityFuel, 0, Point3D(0, 0, 0), "No Model")
{
}

Controller *FuelPickUpController::Replicate(void) const
{
	return (new FuelPickUpController(*this));
}

bool FuelPickUpController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

void FuelPickUpController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);
	data << originalTransform;
}

void FuelPickUpController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);
	data >> originalTransform;
}

int32 FuelPickUpController::GetSettingCount(void) const
{
	return(0);
}

Setting *FuelPickUpController::GetSetting(int32 index) const
{
	return (nullptr);
}

void FuelPickUpController::SetSetting(const Setting *setting)
{
}

void FuelPickUpController::Preprocess(void)
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

RigidBodyStatus FuelPickUpController::HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody)
{
	if (contactBody->GetControllerType() == kControllerPlayer && TheMessageMgr->Server())
	{
		TheMessageMgr->SendMessage(kPlayerServer, ClientPickUpMessage(kFuelPickUpMessage, contactBody->GetControllerIndex(),0));
		TheMessageMgr->SendMessageAll(PickUpObtainedMessage(GetControllerIndex())); //Deletes Pickup for all players
		return kRigidBodyDestroyed;
	}

	return kRigidBodyUnchanged;
}

void FuelPickUpController::ReceiveMessage(const ControllerMessage *message)
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

ControllerMessage *FuelPickUpController::ConstructMessage(ControllerMessageType type) const
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



void FuelPickUpController::Move(void)
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