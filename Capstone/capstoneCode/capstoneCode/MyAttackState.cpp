#include "MyAttackState.h"
#include "MyAIController.h"
#include <math.h>
using namespace C4;

MyAttackState::MyAttackState(MyAIController * _cont) : MyBaseState(_cont)
{
}


MyAttackState::~MyAttackState()
{
}

void MyAttackState::StateEnter(unsigned_int32 _previous)
{
	//Send state update message to everyone
	TheMessageMgr->SendMessageAll(ChangeAIStateMessage(cont->GetControllerIndex(), AIAttackState));
}

void MyAttackState::StateUpdate()
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
	float distance = Magnitude(directionvec);

	if(distance > 8.0F)
	{
		cont->ChangeState(AIChaseState);
	}
	else
	{
		directionvec.Normalize();
		Vector3D vec = cont->GetLinearVelocity();

		Vector3D movementVector = Vector3D(
			directionvec.x*6.0F,
			directionvec.y*6.0F,
			vec.z);
		if (distance < 3.0F)
		{
			cont->SetLinearVelocity(Zero3D);
		}			
		else
			cont->SetLinearVelocity(movementVector);

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

		if (Dot(rightVec, directionvec) <= 0)
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


		//ATTACK PLAYER
		attackTimer += TheTimeMgr->GetDeltaTime();
		if (attackTimer >= 750)
		{
			attackTimer = 0;
			CollisionData	data;

			const Point3D& position = cont->GetTargetNode()->GetWorldPosition(); //Position of player who is attacking
			Vector3D forwardVector = cont->GetTargetNode()->GetNodeTransform()[0];

			Point3D p2 = Point3D(position.x + 10 * forwardVector.x, position.y + 10 * forwardVector.y, position.z + 10); //Position the AI is facing

			if (TheWorldMgr->GetWorld()->QueryCollision(position, p2, 1.0F, kCollisionRigidBody, &data, cont)) // if another rigid body is within range
			{
				MyPlayerController *collisionController = static_cast<MyPlayerController *>(data.rigidBody);
				//Do something about it
				if (collisionController->getType() == kEntityPlayer) // if the rigidBody was a player entity
				{
					//Send attacking sound to everyone
					Point3D Loc = cont->GetTargetNode()->GetWorldPosition();
					SoundMessage sMessage(kMessageSound, kPunchSound, 100.0f, Loc);
					TheMessageMgr->SendMessageAll(sMessage);

					//Create sparks
					SpawnParticleMessage particleMessage(data.position, 100);
					TheMessageMgr->SendMessageAll(particleMessage);

					//Damage the player that got hit
					ServerHealthUpdateMessage message(kMessageHealthUpdate, collisionController->GetControllerIndex(), 5, kEntityPlayer, NULL); 
					TheMessageMgr->SendMessage(kPlayerServer,message);
				}

			}
		}

	}

}

void MyAttackState::StateExit(unsigned_int32 _next)
{
}
