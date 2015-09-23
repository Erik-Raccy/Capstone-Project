#include "MyChaseState.h"
#include "MyAIController.h"

using namespace C4;

MyChaseState::MyChaseState(MyAIController * _cont) : MyBaseState(_cont)
{

}


MyChaseState::~MyChaseState()
{
}

void MyChaseState::StateEnter(unsigned_int32 _previous)
{
	//Send state update message to everyone
	TheMessageMgr->SendMessageAll(ChangeAIStateMessage(cont->GetControllerIndex(), AIChaseState));

	//Depending on previous state play a differennt sound
	if (_previous == AIAttackState)
	{
		Point3D Loc = cont->GetTargetNode()->GetWorldPosition();

		//Send sound to veryon
		SoundMessage sMessage(kMessageSound, kComeBackSound, 100.0f, Loc);
		TheMessageMgr->SendMessageAll(sMessage);
	}
	else
	{
		Point3D Loc = cont->GetTargetNode()->GetWorldPosition();

		//Send sound to veryon
		SoundMessage sMessage(kMessageSound, kAIChaseSound, 100.0f, Loc);
		TheMessageMgr->SendMessageAll(sMessage);
	}
}

void MyChaseState::StateUpdate()
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

	if (Magnitude(directionvec) < 8)
	{
		cont->ChangeState(AIAttackState);
	}
	if(Magnitude(directionvec) > 25)
	{
		cont->ChangeState(AIIdleState);
	}
	else
	{
		directionvec.Normalize();
		Vector3D vec = cont->GetLinearVelocity();

		Vector3D movementVector = Vector3D(
			directionvec.x*6.0F,
			directionvec.y*6.0F,
			vec.z);

		cont->SetLinearVelocity(movementVector);

		//forwardVector is the ai forward
		//directionvec the  vector to the player

		Matrix3D	rotation;
		float angle;

		////COMPUTE AZIMUTH FOR AI
		Point3D point1 = cont->GetTargetNode()->GetNodePosition() + cont->originalForward.Normalize();
		Point3D point2 = cont->GetTargetNode()->GetNodePosition() + directionvec.Normalize();

		float sideLength = Magnitude(point1 - point2);

		// Cosine law

		float cosC = (2.0F - (sideLength*sideLength)) / 2.0F;
		angle = std::acos(cosC);
		Vector3D upVec = Vector3D(0, 0, 1);
		Vector3D rightVec = Cross(cont->originalForward, upVec);
	
		if (Dot(rightVec, directionvec)<=0)
		{
		}
		else
		{
			angle = -angle;
		}
		if (angle < -K::pi) angle += K::two_pi;
		else if (angle > K::pi) angle -= K::two_pi;

		rotation.SetRotationAboutZ(angle);
		cont->SetRigidBodyMatrix3D(Transform(cont->GetTargetNode()->GetSuperNode()->GetInverseWorldTransform(), rotation));
		cont->GetFirstShape()->SetNodeTransform(TransposeTransform(rotation, cont->GetFirstShape()->GetNodeTransform()));


	}

}

void MyChaseState::StateExit(unsigned_int32 _next)
{

}
