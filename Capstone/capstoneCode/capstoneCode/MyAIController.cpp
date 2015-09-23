#include "MyAIController.h"

using namespace C4;
MyAIController::MyAIController() : RigidBodyController(kControllerAI),
EntityEnemies(kEntityEnemy, Zero3D, 30.F, 30.F, 50.F, 50.0F, 10.F, "No model")
{
	FrameAnimatorObserver<MyAIController> *frameObserver = new FrameAnimatorObserver<MyAIController>(this, &MyAIController::AnimationCallback);
	frameAnimator.SetObserver(frameObserver);

	MyIdleState* idle = new MyIdleState(this);
	MyChaseState* angry = new MyChaseState(this);
	MyAttackState* attack = new MyAttackState(this);
	MyAIMap.insert(std::pair<unsigned_int32, MyBaseState *>(AIIdleState, idle));
	MyAIMap.insert(std::pair<unsigned_int32, MyBaseState *>(AIChaseState, angry));
	MyAIMap.insert(std::pair<unsigned_int32, MyBaseState *>(AIAttackState, attack));
	CurrentState = idle;
}

MyAIController::~MyAIController()
{
	MyAIMap.clear();
}

//copy constructor, needed for world editor
MyAIController::MyAIController(const MyAIController& myController) : RigidBodyController(myController),
EntityEnemies(kEntityEnemy, Zero3D, 50.F, 50.F, 50.F, 50.0F, 10.F, "No model")
{
	//values that need to be copied
	turnMagnitude = myController.turnMagnitude;
	moveMagnitude = myController.moveMagnitude;
	originalTransform = myController.originalTransform;
}

//replicate that calls copy constructor, also needed for world editor
Controller *MyAIController::Replicate(void) const
{
	return (new MyAIController(*this));
}

//specifies what type of objects the controller can be used on
//Notice that the ValidNode function is declared static. The World Editor calls this function automatically when it needs to know whether the controller can be used with a specific node.
bool MyAIController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

//pack and unpack are needed so that the controller can be written to a file and later restored
void MyAIController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);

	data << turnMagnitude;
	data << moveMagnitude;
	// Write the original transform
	data << originalTransform;
}

void MyAIController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);

	data >> turnMagnitude;
	data >> moveMagnitude;
	// Write the original transform
	data >> originalTransform;
}

//The Controller object is queried by the World Editor for its configurable settings 
//The GetSettingCount function returns the number of individual settings that need to be shown,
int32 MyAIController::GetSettingCount(void) const
{
	return (2);
}


Setting *MyAIController::GetSetting(int32 index) const
{
	if (index == 0)
	{
		// Yes, return a new text setting and set its value in revolutions per second
		return (new TextSetting('move', Text::FloatToString(moveMagnitude),
			"Move Multiplier", 7, &EditTextWidget::FloatNumberFilter));
	}
	if (index == 1)
	{
		// Yes, return a new text setting and set its value in revolutions per second
		return (new TextSetting('turn', Text::FloatToString(turnMagnitude),
			"Turn Sensitivity", 7, &EditTextWidget::FloatNumberFilter));
	}
	return (nullptr);
}

void MyAIController::SetSetting(const Setting *setting)
{
	if (setting->GetSettingIdentifier() == 'move')
	{
		// Yes, grab the value from the setting and convert it back to radians per millisecond
		const char *text = static_cast<const TextSetting *>(setting)->GetText();
		moveMagnitude = Text::StringToFloat(text);
	}
	if (setting->GetSettingIdentifier() == 'turn')
	{
		// Yes, grab the value from the setting and convert it back to radians per millisecond
		const char *text = static_cast<const TextSetting *>(setting)->GetText();
		turnMagnitude = Text::StringToFloat(text);
	}
}

void MyAIController::SetMovementFlag(Vector2D moveVec)
{
	inputVector = moveVec;
}


//The Preprocess function is called once when the controller's target node is inserted into a scene
void MyAIController::Preprocess(void)
{

	RigidBodyController::Preprocess();

	originalForward = GetTargetNode()->GetNodeTransform()[0];
	//needed for Sendplayerlist
	this->SetID(this->GetControllerIndex());

	// Grab the original transform of the target node
	const Node *target = GetTargetNode();
	originalTransform = target->GetNodeTransform();

	SetRigidBodyFlags(kRigidBodyKeepAwake | kRigidBodyFixedOrientation);
	SetFrictionCoefficient(0.5F);

	Model *model = GetTargetNode();
	frameAnimator.SetTargetModel(model);
	model->SetRootAnimator(&frameAnimator);

	Interpolator *interpolator = frameAnimator.GetFrameInterpolator();
	frameAnimator.SetAnimation("Models/AI/AIStill");
	interpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
}

//The Move function is called once per frame to let the controller move its target node
void MyAIController::Move(void)
{
	if (!AIDead) //death animation hack1
	{

		//State machine code
		if (TheMessageMgr->Server())
		{
			if (initialized)
				return;

			if (CurrentState != NULL)
				CurrentState->StateUpdate();

			if (NextStateIndex != AINullState)
			{
				if (CurrentState != NULL)
					CurrentState->StateExit(NextStateIndex);
				CurrentState = MyAIMap[NextStateIndex];
				CurrentState->StateEnter(CurrentStateIndex);

				if (CurrentStateIndex != NextStateIndex)
					AnimateState(NextStateIndex);

				CurrentStateIndex = NextStateIndex;
				NextStateIndex = AINullState;
			}

			Point3D temp = GetTargetNode()->GetNodePosition();
			if (temp.z < -1000)
				TheMessageMgr->SendMessageAll(PlayerDestroyMessage(GetControllerIndex()));
		}
		//end state machine code
		MessageTimer += TheTimeMgr->GetDeltaTime();
		if (TheMessageMgr->Server() && CurrentStateIndex != AIIdleState && MessageTimer >= 33)
		{
			AIMovementMessage msg = AIMovementMessage(kAIMessageState, this->GetControllerIndex(), this->GetTargetNode()->GetNodePosition(), GetLinearVelocity());
			TheMessageMgr->SendMessageClients(msg);
			MessageTimer = 0;
		}

	} //end of hack1

	SetPosition(this->GetTargetNode()->GetNodePosition());//needed for network spawns to be correct
	GetTargetNode()->Animate();

	if (Deletable) // death animation hack 2
		Destroy();
}

void MyAIController::AnimateState(int32 state)
{
	Interpolator *interpolator = frameAnimator.GetFrameInterpolator();

	switch (state)
	{
		case AIIdleState:
		{
			frameAnimator.SetAnimation("Models/AI/AIStill");
			interpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
			break;
		}
		case AIChaseState:
		{
			//Engine::Report("CHASING ANIMIATION");
			frameAnimator.SetAnimation("Models/AI/AIWalk");
			interpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
			break;
		}
		case AIAttackState:
		{
			frameAnimator.SetAnimation("Models/AI/AIKick");
			interpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
			break;
		}
	}
}

void MyAIController::ChangeState(unsigned_int32 _NextStateIndex)
{
	NextStateIndex = _NextStateIndex;
}

//on collision
RigidBodyStatus MyAIController::HandleNewGeometryContact(const GeometryContact *contact)
{
	RigidBodyStatus temp = RigidBodyStatus();
	return temp;
}


void MyAIController::ReceiveMessage(const ControllerMessage *message)
{

	switch (message->GetControllerMessageType())
	{
		case kAIMessageState:
		{
			const AIMovementMessage *m = static_cast<const AIMovementMessage *>(message);
			SetRigidBodyPosition(m->GetInitialPosition());
			SetLinearVelocity(m->GetInitialVelocity());
			break;
		}
		case kNodeMessageState:
		{
			const PlayerStateMessage *msg = static_cast<const PlayerStateMessage *>(message);
			GetTargetNode()->SetNodePosition(msg->GetPosition());
			break;
		}
		case kPlayerMessageDestroy:
		{
			AIDead = true;
			Interpolator *interpolator = frameAnimator.GetFrameInterpolator();
			frameAnimator.SetAnimation("Models/AI/AIDeath");
			interpolator->SetMode(kInterpolatorForward);
			//Destroy();
			break;
		}
		case kChangeStateMessage:
		{
			const ChangeAIStateMessage *msg = static_cast<const ChangeAIStateMessage *>(message);
			AnimateState(msg->GetState());
			break;
		}
		case kPlayerMessageBeginMovement:
		{
			break;
		}
		case kPlayerMessageEndMovement:
		{

			break;
		}
		case kPlayerMessageOrientation:
		{
		}
	}

	RigidBodyController::ReceiveMessage(message);
}

ControllerMessage *MyAIController::ConstructMessage(ControllerMessageType type) const
{
	switch (type)
	{
		
		case kAIMessageState:
		{
			return (new AIMovementMessage(type, GetControllerIndex()));
		}
		case kNodeMessageState:
		{
			return (new PlayerStateMessage(GetControllerIndex()));
		}
		case kPlayerMessageDestroy:
		{
			return (new PlayerDestroyMessage(GetControllerIndex()));
		}
		case kChangeStateMessage:
		{
			return (new ChangeAIStateMessage(GetControllerIndex(), AINullState));
		}
		case kPlayerMessageBeginMovement:
			break;
		case kPlayerMessageEndMovement:
			break;
		case kPlayerMessageOrientation:
		{
			break;
		}
	}
	return (RigidBodyController::ConstructMessage(type));
}

void MyAIController::AnimationCallback(FrameAnimator *frame, CueType cue)
{
	if (cue == 'Deth')
	{
		Deletable = true;
	}
}


void MyAIController::Destroy()
{
	Node *node = this->GetTargetNode();
	if (node)
	{
		delete node;
	}
}