#include "PickUpController.h"

using namespace C4;
//base calss constructors need to be called if you want it to pop up in world editor
PickUpController::PickUpController() : RigidBodyController(kControllerPickUp), EntityPowerups(kEntityPowerup, 0.F, Point3D(0,0,0), "No Model")
{
}

PickUpController::~PickUpController()
{
}

//copy constructor, also needed for world editor
PickUpController::PickUpController(const PickUpController& myController) : spawntype(myController.GetType()), RigidBodyController(myController), EntityPowerups(kEntityPowerup, 0.F, Point3D(0, 0, 0), "No Model")
{
}

//replicate that calls copy constructor, also needed for world editor
Controller *PickUpController::Replicate(void) const
{
	return (new PickUpController(*this));
}

//specifies what type of objects the controller can be used on
//Notice that the ValidNode function is declared static. The World Editor calls this function automatically when it needs to know whether the controller can be used with a specific node.
bool PickUpController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

//pack and unpack are needed so that the controller can be written to a file and later restored
void PickUpController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);
	// Write the original transform
	data << originalTransform;
}

void PickUpController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);
	data >> originalTransform;
}

//The Controller object is queried by the World Editor for its configurable settings 
//The GetSettingCount function returns the number of individual settings that need to be shown,
int32 PickUpController::GetSettingCount(void) const
{
	// There's only one setting
	return (0);
}


Setting *PickUpController::GetSetting(int32 index) const
{
	return (nullptr);
}

void PickUpController::SetSetting(const Setting *setting)
{
}

//The Preprocess function is called once when the controller's target node is inserted into a scene
void PickUpController::Preprocess(void)
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

RigidBodyStatus PickUpController::HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody)
{
	//Engine::Report("blah");
	if (contactBody->GetControllerType() == kControllerPlayer)
	{
		//Engine::Report("Default Pickup Obtained");
		TheMessageMgr->SendMessageAll(PickUpObtainedMessage(GetControllerIndex())); //Deletes Pickup for all players
		return kRigidBodyDestroyed;
	}

	return kRigidBodyUnchanged;
}

void PickUpController::ReceiveMessage(const ControllerMessage *message)
{
	switch (message->GetControllerMessageType())
	{
	case kPickUpObtainedMessage:
		{
		for (int i = 0; i < ThePlayerManager->EntityList.size(); i++)
		{
			if (ThePlayerManager->EntityList[i]->getType() == kEntityPowerup) //enum changes based on pickup type
			{
				PickUpController *cont = static_cast<PickUpController*>(ThePlayerManager->EntityList[i]); //controller changes based on type
				if (cont->GetControllerIndex() == this->GetControllerIndex())
				{
					ThePlayerManager->EntityList.erase(ThePlayerManager->EntityList.begin()+i);
				}
			}
		}
			const PickUpObtainedMessage *m = static_cast<const PickUpObtainedMessage *>(message);
			Node *target = GetTargetNode();
			delete target;
			break;
		}
	}
}

ControllerMessage *PickUpController::ConstructMessage(ControllerMessageType type) const
{
	switch (type)
	{
	case kPickUpObtainedMessage:
		return (new PickUpObtainedMessage(GetControllerIndex()));
	}
	return (RigidBodyController::ConstructMessage(type));
}