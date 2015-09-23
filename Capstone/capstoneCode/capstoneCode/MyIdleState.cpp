#include "MyIdleState.h"
#include "MyAIController.h"

using namespace C4;

MyIdleState::MyIdleState(MyAIController * _cont) : MyBaseState(_cont)
{
}


MyIdleState::~MyIdleState()
{
}

void MyIdleState::StateEnter(unsigned_int32 _previous)
{
	//Send state update message to everyone
	TheMessageMgr->SendMessageAll(ChangeAIStateMessage(cont->GetControllerIndex(), AIIdleState));
}

void MyIdleState::StateUpdate()
{
	//forward vector
	Vector3D forwardVector = cont->GetTargetNode()->GetNodeTransform()[0];
	//right vector
	Vector3D rightVector = cont->GetTargetNode()->GetNodeTransform()[1];
	//rotate the player
	Vector2D inputVector = Vector2D(0, 0);
	// move player
	Vector3D currentpos = cont->GetTargetNode()->GetNodePosition();
	EntityBase * temp = ThePlayerManager->GetClosestEntity(currentpos);
	Vector3D targetpos;
	if (temp)
		targetpos = temp->GetPosition();
	else
		targetpos = currentpos;
	
	Vector3D directionvec = targetpos - currentpos;
	directionvec.z = 0;
	
	if (Magnitude(directionvec) < 25 && Magnitude(directionvec) > 12) //go towards the player
	{	
		cont->ChangeState(AIChaseState); //switch to agro mode
	}
	else
	{
		//dont move towards player
		Vector3D vec = cont->GetLinearVelocity();
		Vector3D movementVector = Vector3D(0, 0, vec.z);
		cont->SetLinearVelocity(movementVector);
	}

}

void MyIdleState::StateExit(unsigned_int32 _next)
{
	

}
