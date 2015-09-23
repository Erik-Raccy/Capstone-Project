#ifndef MYPLAYERCONTROLLER_H
#define MYPLAYERCONTROLLER_H

/*
* File:  MyPlayerController.h
* --------------------
* This file contains the Controller class used
* by the player. parts of this code are run locally
* on the clients, some are run only on the server.
*/


//Engine Headers
#include "C4Character.h"
#include "C4Physics.h"
#include "C4Configuration.h"
//#include  "MyMessages.h"

//My headers
#include "Game.h"
#include "EntityBase.h"
#include "EntityPlayer.h"

namespace C4
{
	enum animationFlag
	{
		kForwardAnimation = 0,
		kBackwardAnimation = 1,
		kLeftAnimation,
		kRightAnimation,
		kIdleAnimation,
		kAttackAnimation,
		kChainsawAnimation,
		kCrouchAnimation,
		kRiseAnimation,
	};

	class MyPlayerController : public CharacterController, public EntityPlayer
	{
		friend class MyPlayerManager;
	public:

		//constructor
		MyPlayerController(); //constructor
		~MyPlayerController();
		MyPlayerController(const MyPlayerController& myController); //copy constructor
		//accessors
		Model *GetTargetNode(void) const
		{
			return (static_cast<Model *>(Controller::GetTargetNode()));
		}
		
		//methods
		static bool ValidNode(const Node *node);//validate node for editor
		int32 GetSettingCount(void) const;// User interface functions for world editor
		Setting *GetSetting(int32 index) const;
		void SetSetting(const Setting *setting);
		void Pack(Packer& data, unsigned_int32 packFlags) const;// Serialization functions for save/load 
		void Unpack(Unpacker& data, unsigned_int32 unpackFlags);
		void Preprocess(void) override;
		void Move(void) override;// The function that moves the target node
		void Destroy(void);
		RigidBodyStatus HandleNewGeometryContact(const GeometryContact *contact) override; // rigidbody contact
		//message methods
		void BeginMovement(unsigned_int32 flag);
		void EndMovement(unsigned_int32 flag);
		void BeginOrientation(float orientation, Player * sender);
		void ReceiveMessage(const ControllerMessage *message);
		ControllerMessage *ConstructMessage(ControllerMessageType type) const;
		//my methods
		void SetMovementFlag(unsigned_int32 flag);
		void SetOrientation(float _orientation);
		void SetPlayerOrientation(float azi, float roll=0); //Function that rotates the player
		//Entity methods

		PlayerKey getPlayerKey();

		Interpolator *mainFrameInterpolator;
		Interpolator *mainWeightInterpolator;
		Interpolator *secondFrameInterpolator;		
		Interpolator *secondWeightInterpolator;
		Interpolator *armFrameInterpolator;
		Interpolator *armWeightInterpolator;

		animationFlag newAnimation;
		animationFlag oldAnimation;
		void SetSelfDestruct();
		bool GetSelfDestruct(){
			return selfDestructActive;
		}
		bool Crouching = false;

		Sound *sound;
		bool soundPlaying;

		const float CHAINSAWCOOLDOWN = 1900;
		const float PUNCHCOOLDOWN = 600;
		float cooldownCounter;
		float cooldown = PUNCHCOOLDOWN;
		bool attacking = false;

		virtual void OnDeath();
		
	protected:
	
	
	private:
		//methods
		
		Controller *Replicate(void) const; //replicate method
		
		//variables
		Transform4D		originalTransform;    // The target's original transform
		Vector3D		forwardVector;
		Vector3D		rightVector;
		Vector2D		inputVector;
		float			orientation;
		float			turnMagnitude;
		float			moveMagnitude;
		float			jumpMagnitude;
		bool			jumping;
		float			fuelModifier;
		float				modelAzimuth; // The azimuth and altitude represent the direction the player is looking
		float				modelAltitude;
		long orientationMessageTimer;	//networking
		long clientInformationMessageTimer;
		long soundWalkUpdateTimer;
		bool selfDestructActive;
		float selfDestructTimer;
		float alarmSoundTimer = -1000;

		void AnimationCallback(FrameAnimator *frame, CueType type);
		FrameAnimatorObserver<FrameAnimator> *animationObserver;
		void SetMotionFlag(float x, float y);
		void SetPlayerMotion(animationFlag newAni, animationFlag oldAni);
		const char* DirectoryConverter(animationFlag flag);
		FrameAnimator mainFrameAnimator;
		FrameAnimator secondFrameAnimator;
		FrameAnimator armFrameAnimator;
		BlendAnimator *blendAnimator;
		MergeAnimator *mergeAnimator;
	};
}

#endif