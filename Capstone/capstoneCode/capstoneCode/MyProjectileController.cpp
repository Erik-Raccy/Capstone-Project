#include "MyProjectileController.h"

using namespace C4;
//base class constructors need to be called if you want it to pop up in world editor
MyProjectileController::MyProjectileController(unsigned_int32 _shotType, Vector3D _velocity) : RigidBodyController(kControllerProjectile), EntityBullet(kEntityBullet)
{
	projectileType = _shotType;
	velocity = _velocity;
	killTimer = 0;
}

MyProjectileController::MyProjectileController() : RigidBodyController(kControllerProjectile), EntityBullet(kEntityBullet)
{

}

MyProjectileController::~MyProjectileController()
{

}

//copy constructor, also needed for world editor
MyProjectileController::MyProjectileController(const MyProjectileController& myController) : RigidBodyController(myController), EntityBullet(kEntityBullet)
{
	velocity = myController.velocity;
}

//replicate that calls copy constructor, also needed for world editor
RigidBodyController *MyProjectileController::Replicate(void) const
{
	return (new MyProjectileController(*this));
}

//specifies what type of objects the controller can be used on
//Notice that the ValidNode function is declared static. The World Editor calls this function automatically when it needs to know whether the controller can be used with a specific node.
bool MyProjectileController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeModel);
}

//pack and unpack are needed so that the controller can be written to a file and later restored
void MyProjectileController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);

	data << velocity;
}

void MyProjectileController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);

	data >> velocity;
}

//The Controller object is queried by the World Editor for its configurable settings 
//The GetSettingCount function returns the number of individual settings that need to be shown,
int32 MyProjectileController::GetSettingCount(void) const
{
	//// There's only one setting
	return (0);
}


Setting *MyProjectileController::GetSetting(int32 index) const
{
	return (nullptr);
}

void MyProjectileController::SetSetting(const Setting *setting)
{

}

//The Preprocess function is called once when the controller's target node is inserted into a scene
void MyProjectileController::Preprocess(void)
{
	RigidBodyController::Preprocess();
	
	// Grab the original transform of the target node
	const Node *target = GetTargetNode();
	originalTransform = target->GetNodeTransform();

	SetRigidBodyFlags(kRigidBodyKeepAwake);
	this->SetFrictionCoefficient(0.5F);
	this->SetLinearVelocity(60*velocity);
	this->SetGravityMultiplier(0.02);
}

//The Move function is called once per frame to let the controller move its target node
void MyProjectileController::Move(void)
{
	if (killTimer >= 2000)
	{
		delete this->GetTargetNode();
	}
	killTimer += TheTimeMgr->GetDeltaTime();
}

RigidBodyStatus MyProjectileController::HandleNewRigidBodyContact(const RigidBodyContact *contact, RigidBodyController *contactBody)
{
	if (projectileType == k_weaponGun){
		if (contactBody->GetControllerType() == kControllerPlayer)
		{
			if (TheMessageMgr->Server())
			{
				SpawnParticleMessage particleMessage(this->GetTargetNode()->GetNodePosition(), 100);
				TheMessageMgr->SendMessageAll(particleMessage);
				DestroyProjectileMessage message(this->GetControllerIndex());
				TheMessageMgr->SendMessageAll(message);
				ServerHealthUpdateMessage healthMessage(kMessageHealthUpdate, contactBody->GetControllerIndex(), WEAPON_ROCKETLAUNCHER_DAMAGE, kEntityPlayer, NULL);
				TheMessageMgr->SendMessage(kPlayerServer, healthMessage);
			}
		}
		if (contactBody->GetControllerType() == kControllerAI)
		{
			if (TheMessageMgr->Server())
			{
				SpawnParticleMessage particleMessage(this->GetTargetNode()->GetNodePosition(), 100);
				TheMessageMgr->SendMessageAll(particleMessage);
				DestroyProjectileMessage message(this->GetControllerIndex());
				TheMessageMgr->SendMessageAll(message);
				ServerHealthUpdateMessage healthMessage(kMessageHealthUpdate, contactBody->GetControllerIndex(), WEAPON_ROCKETLAUNCHER_DAMAGE, kEntityEnemy,NULL);
				TheMessageMgr->SendMessage(kPlayerServer, healthMessage);
			}
		}
	}

	else if (projectileType == k_weaponSelfDestructGun)
	{
		MyPlayerController * hitPlayer = static_cast<MyPlayerController *>(contactBody);
		if (hitPlayer->GetSelfDestruct() == false){
			hitPlayer->SetSelfDestruct();
		}
		DestroyProjectileMessage message(this->GetControllerIndex());
		TheMessageMgr->SendMessageAll(message);
	}
	return kRigidBodyDestroyed;
}

RigidBodyStatus MyProjectileController::HandleNewGeometryContact(const RigidBodyContact *contact)
{
	DestroyProjectileMessage message(this->GetControllerIndex());
	TheMessageMgr->SendMessageAll(message);
	return kRigidBodyDestroyed;
}

void MyProjectileController::ReceiveMessage(const ControllerMessage *message)
{
	switch (message->GetControllerMessageType())
	{
	case kDestroyProjectileMessage:
	{
		Node *node = this->GetTargetNode();
		ThePlayerManager->RemoveEntity(this->GetControllerIndex());
		if (node)
		{
			delete node;
		}
		break;
	}
		RigidBodyController::ReceiveMessage(message);
	}
}
ControllerMessage *MyProjectileController::ConstructMessage(ControllerMessageType type) const
{
	switch (type)
	{
	case kDestroyProjectileMessage:
		return (new DestroyProjectileMessage(GetControllerIndex()));
	}
	return (RigidBodyController::ConstructMessage(type));
}