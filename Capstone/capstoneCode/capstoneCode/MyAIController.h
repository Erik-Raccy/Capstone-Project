#ifndef MYAICONTROLLER_H
#define MYAICONTROLLER_H


/*
* File:  MyAIController.h
* --------------------------------
* This file contains the AI controller class
* this class controls the behavious of the AI
* entities that appear in the game. The controller's
* behavious is controlled via the state classes which
* are contained in a map within this class.
*/


//Engine Headers
#include "C4Physics.h"
#include "C4Engine.h"

//My headers
#include <map>
#include "Game.h"
#include "EntityEnemies.h"
#include "MyBaseState.h"
#include "MyIdleState.h"
#include "MyChaseState.h"
#include "MyAttackState.h"

namespace C4
{
	enum AIState
	{
		AINullState = 0,
		AIIdleState = 1,
		AIChaseState,
		AIAttackState
	};
	

	class MyAIController : public RigidBodyController, public EntityEnemies
	{
	public:

		//constructors
		MyAIController();
		~MyAIController();
		MyAIController(const MyAIController& myController); //copy constructor

		//validate node for editor
		static bool ValidNode(const Node *node);

		// Serialization functions for save/load
		void Pack(Packer& data, unsigned_int32 packFlags) const;
		void Unpack(Unpacker& data, unsigned_int32 unpackFlags);

		// User interface functions for world editor
		int32 GetSettingCount(void) const;
		Setting *GetSetting(int32 index) const;
		void SetSetting(const Setting *setting);

		//my methods
		void SetMovementFlag(Vector2D _moveVec);
		
		Model *GetTargetNode(void) const
		{
			return (static_cast<Model *>(Controller::GetTargetNode()));
		}
		float GetMoveMagnitude(void)
		{
			return moveMagnitude;
		}
		void ChangeState(unsigned_int32 _NextStateIndex);
		//default methods
		void Preprocess(void) override;
		void Move(void) override;// The function that moves the target node
		void AnimateState(int32 state);
		void Destroy(void);
		bool AIDead = false; //used to start death animation hack1
		//message stuff
		void BeginMovement(unsigned_int32 flag);
		void EndMovement(unsigned_int32 flag);
		void BeginOrientation(float orientation);
		//messages
		void ReceiveMessage(const ControllerMessage *message);
		ControllerMessage *ConstructMessage(ControllerMessageType type) const;
		
		RigidBodyStatus HandleNewGeometryContact(const GeometryContact *contact) override;
		Vector3D		originalForward;
	private:

		Transform4D		originalTransform;    // The target's original transform
		
		Vector3D		rightVector;
		Vector2D		inputVector = Vector2D(0,0);
		float			orientation;
		float			turnMagnitude = 5;
		float			moveMagnitude = 40.0F;
		
		// The azimuth and altitude represent the direction the player is looking
		// by using the mouse.
		float				modelAzimuth;
		float				modelAltitude;
		float				MessageTimer = 0;
		//AI state stuff
		MyBaseState * CurrentState = nullptr;
		unsigned_int32 CurrentStateIndex = AINullState;
		unsigned_int32 NextStateIndex = AINullState;
		unsigned_int32 PreviousStateIndex = AINullState;

		std::map<unsigned_int32, MyBaseState *> MyAIMap;

		bool initialized = false;
		
		bool Deletable = false; // used to delete AI upon animation finish hack2

		//needed for editor
		Controller *Replicate(void) const; //replicate method
		//networking
		long orientationMessageTimer;
		void HandleAnimationEvent(int32 cue);
		void AnimationCallback(FrameAnimator *frame, CueType type);
		FrameAnimatorObserver<FrameAnimator> *animationObserver;

		FrameAnimator frameAnimator;
	};

}

#endif