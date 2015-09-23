#include "MyPlayerController.h"
#include "MainMenu.h"
//#include"RespawnMenu.h"
#include <string>
using namespace C4;

//base class constructors need to be called if you want it to pop up in world editor
MyPlayerController::MyPlayerController() : CharacterController(kControllerPlayer), EntityPlayer(kEntityPlayer, Zero3D, 100.F, 100.F, 100.F, 100.F, 0.F, "No model")
{
	FrameAnimatorObserver<MyPlayerController> *frameObserver = new FrameAnimatorObserver<MyPlayerController>(this, &MyPlayerController::AnimationCallback);
	mainFrameAnimator.SetObserver(frameObserver);

	inputVector = Vector2D(0, 0);
	moveMagnitude = 7;
	turnMagnitude = 100;
	jumpMagnitude = 300;
	jumping = false;
	fuelModifier = 0.0005F;
	fuel = 100;
	mFuel = 100;
	mHealth = 100;
	mTotalHealth = 100;
	mScraps = 10000;
	SetCollisionExclusionMask(kCollisionCamera);
	cooldown = 0;
	soundWalkUpdateTimer = 1000;
	setScraps(100);
	selfDestructActive = false;
	selfDestructTimer = 0;

	///* UNCOMMENT THE FOLLOWING TO UNLOCK ALL WEAPONS AND ARMOR*/
	//EntityWeapon *blender = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_BLENDER_DAMAGE, WEAPON_BLENDER_KNOCKBACK, WEAPON_BLENDER_RANGE, nullptr, k_weaponBlender);
	//AddItemToInventory(blender);

	//EntityWeapon *rocketLauncher = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_BLENDER_DAMAGE, WEAPON_BLENDER_KNOCKBACK, WEAPON_BLENDER_RANGE, nullptr, k_weaponGun);
	//AddItemToInventory(rocketLauncher);

	//EntityArmor *armor = new EntityArmor(kEntityArmor, ARMOR_DIAMOND_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorDiamond);
	//AddItemToInventory(armor);

	//EntityArmor *armor2 = new EntityArmor(kEntityArmor, ARMOR_STEEL_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorSteel);
	//AddItemToInventory(armor2);

	//EntityArmor *armor3 = new EntityArmor(kEntityArmor, ARMOR_BRONZE_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorBronze);
	//AddItemToInventory(armor3);

	//EntityArmor *armor4 = new EntityArmor(kEntityArmor, ARMOR_TITANIUM_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorTitanium);
	//AddItemToInventory(armor4);

	//EntityArmor *armor5 = new EntityArmor(kEntityArmor, ARMOR_TITANIUM_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorIron);
	//AddItemToInventory(armor5);
	//
	//EntityArmor *armor6 = new EntityArmor(kEntityArmor, ARMOR_TITANIUM_DEFENSE, Point3D(NULL, NULL, NULL), nullptr, k_ArmorGold);
	//AddItemToInventory(armor6);

	//EntityWeapon *chainSaw = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), WEAPON_CHAINSAW_DAMAGE, WEAPON_CHAINSAW_KNOCKBACK, WEAPON_CHAINSAW_RANGE, nullptr, k_weaponChainsaw);
	//AddItemToInventory(chainSaw);

	//EntityWeapon *selfDestructGun = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), SELF_DESTRUCT_DAMAGE, SELF_DESTRUCT_DAMAGE, SELF_DESTRUCT_DAMAGE, nullptr, k_weaponSelfDestructGun);
	//AddItemToInventory(selfDestructGun);
}

MyPlayerController::~MyPlayerController()
{
	delete blendAnimator;
}

//copy constructor, also needed for world editor
MyPlayerController::MyPlayerController(const MyPlayerController& myController) : CharacterController(myController), EntityPlayer(kEntityPlayer, Zero3D, 100.F, 100.F, 100.F, 100.F, 0.F, "No model")
{
	//values that need to be copied
	turnMagnitude = myController.turnMagnitude;
	moveMagnitude = myController.moveMagnitude;
	jumpMagnitude = myController.jumpMagnitude;
	originalTransform = myController.originalTransform;
	jumping = myController.jumping;
}

//replicate that calls copy constructor, also needed for world editor
Controller *MyPlayerController::Replicate(void) const
{
	return (new MyPlayerController(*this));
}

//specifies what type of objects the controller can be used on
//Notice that the ValidNode function is declared static. The World Editor calls this function automatically when it needs to know whether the controller can be used with a specific node.
bool MyPlayerController::ValidNode(const Node *node)
{
	return (node->GetNodeType() == kNodeGeometry);
}

//pack and unpack are needed so that the controller can be written to a file and later restored
void MyPlayerController::Pack(Packer& data, unsigned_int32 packFlags) const
{
	Controller::Pack(data, packFlags);

	data << turnMagnitude;
	data << moveMagnitude;
	data << jumpMagnitude;
	// Write the original transform
	data << originalTransform;
}

void MyPlayerController::Unpack(Unpacker& data, unsigned_int32 unpackFlags)
{
	Controller::Unpack(data, unpackFlags);

	data >> turnMagnitude;
	data >> moveMagnitude;
	data >> jumpMagnitude;
	// Write the original transform
	data >> originalTransform;
}

//The Controller object is queried by the World Editor for its configurable settings 
//The GetSettingCount function returns the number of individual settings that need to be shown,
int32 MyPlayerController::GetSettingCount(void) const
{
	return (3);
}

Setting *MyPlayerController::GetSetting(int32 index) const
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
	if (index == 2)
	{
		// Yes, return a new text setting and set its value in revolutions per second
		return (new TextSetting('jump', Text::FloatToString(jumpMagnitude),
			"Jump Height Multiplier", 7, &EditTextWidget::FloatNumberFilter));
	}
	return (nullptr);
}

void MyPlayerController::SetSetting(const Setting *setting)
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
	if (setting->GetSettingIdentifier() == 'jump')
	{
		// Yes, grab the value from the setting and convert it back to radians per millisecond
		const char *text = static_cast<const TextSetting *>(setting)->GetText();
		jumpMagnitude = Text::StringToFloat(text);
	}
}

void MyPlayerController::SetMovementFlag(unsigned_int32 flag)
{
	
	if (flag == kActionForward )
	{
		inputVector.x += 1 ;
	}
	else if (flag == ~kActionForward && inputVector.x != 0 )
	{
		inputVector.x -= 1 ;
	}
	else if (flag == kActionBackward )
	{
		inputVector.x += -1 ;
	}
	else if (flag == ~kActionBackward  && inputVector.x != 0)
	{
		inputVector.x -= -1 ;
	}
	else if (flag == kActionLeft)
	{
		inputVector.y += 1 ;
	}
	else if (flag == ~kActionLeft  && inputVector.y != 0)
	{
		inputVector.y -= 1 ;
	}
	else if (flag == kActionRight)
	{
		inputVector.y += -1 ;
	}
	else if (flag == ~kActionRight  && inputVector.y != 0)
	{
		inputVector.y -= -1 ;
	}
	else if (flag == kActionUp && !jumping)
	{
		this->SetExternalForce(Zero3D);
		this->ApplyImpulse((Vector3D(0, 0, 1) * jumpMagnitude));
		jumping = true;

		GamePlayer *player = static_cast<GamePlayer*>(TheMessageMgr->GetLocalPlayer());

		if (player->GetController() == this)
		{
			// Send the jumping sound to everyone
			Point3D Loc = this->GetTargetNode()->GetWorldPosition();
			SoundMessage sMessage(kMessageSound, kJumpSound, 100.0f, Loc);
			TheMessageMgr->SendMessage(kPlayerAll, sMessage);
		}
	}
	else if (flag == ~kActionUp)
	{
		// do nothing
	}
	else
	{

	}
}

void MyPlayerController::SetOrientation(float _orientation)
{
	orientation = _orientation;
}

//The Preprocess function is called once when the controller's target node is inserted into a scene
void MyPlayerController::Preprocess(void)
{
	RigidBodyController::Preprocess();

	// Grab the original transform of the target node
	const Node *target = GetTargetNode();
	originalTransform = target->GetNodeTransform();

	SetRigidBodyFlags(kRigidBodyKeepAwake | kRigidBodyFixedOrientation);
	SetFrictionCoefficient(0.1F);
	forwardVector = this->GetTargetNode()->GetNodeTransform()[0];

	Model *model = GetTargetNode();
	Node *n = model->FindNode("Body")->GetFirstSubnode()->Next()->Next();

	BlendAnimator *blendAnimator = new BlendAnimator(model);
	MergeAnimator *mergeAnimator = new MergeAnimator(model);

	mainFrameAnimator.SetTargetModel(model);
	secondFrameAnimator.SetTargetModel(model);
	armFrameAnimator.SetTargetModel(model, n);

	mainFrameAnimator.SetAnimation("Models/Player/PlayerIdle");
	secondFrameAnimator.SetAnimation("Models/Player/PlayerIdle");
	armFrameAnimator.SetAnimation("Models/Player/PlayerIdle");

	blendAnimator->AddNewSubnode(&mainFrameAnimator);
	blendAnimator->AddNewSubnode(&secondFrameAnimator);
	mergeAnimator->AddNewSubnode(blendAnimator);
	mergeAnimator->AddNewSubnode(&armFrameAnimator);
	model->SetRootAnimator(mergeAnimator);

	mainFrameInterpolator = mainFrameAnimator.GetFrameInterpolator();
	mainWeightInterpolator = mainFrameAnimator.GetWeightInterpolator();
	secondFrameInterpolator = secondFrameAnimator.GetFrameInterpolator();
	secondWeightInterpolator = secondFrameAnimator.GetWeightInterpolator();
	armFrameInterpolator = armFrameAnimator.GetFrameInterpolator();
	armWeightInterpolator = armFrameAnimator.GetWeightInterpolator();

	mainFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
	mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f/250.0f);
	secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
	secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f/250.0f);
	armFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
	armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
	
	mainFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
	mainWeightInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
	secondFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
	secondWeightInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
	armFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
	armWeightInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
}

//The Move function is called once per frame to let the controller move its target node
void MyPlayerController::Move(void)
{
    // Check if this soldier is the one being controlled by the local player
    GamePlayer *player = static_cast<GamePlayer*>(TheMessageMgr->GetLocalPlayer());

    if (player->GetController() == this)
    {
		if (jumping == false && !(inputVector.x == 0 && inputVector.y == 0) && !attacking && !Crouching)
		{
			SetMotionFlag(inputVector.x, inputVector.y);
			//Send walking sound  to everyone every second
			soundWalkUpdateTimer += TheTimeMgr->GetDeltaTime();
			if (soundWalkUpdateTimer >= 1250)
			{
				Point3D Loc = this->GetTargetNode()->GetWorldPosition();
				SoundMessage sMessage(kMessageSound, kWalkSound, 100.0f, Loc);
				TheMessageMgr->SendMessage(kPlayerAll, sMessage);
				soundWalkUpdateTimer = 0;
			}
		}
		else{
			if (!(Crouching | attacking))
				newAnimation = kIdleAnimation;
			
			soundPlaying = false;
			soundWalkUpdateTimer = 1000;
		}

		//Attack cooldown
		cooldownCounter += TheTimeMgr->GetDeltaTime();
		if (cooldownCounter > cooldown)
			attacking = false;
		// grab the mouse deltas from the Input Manager.
		// We use these to change the angles representing the direction in
		// which the player is looking/moving.

		float azm = modelAzimuth + TheInputMgr->GetMouseDeltaX();
        if (azm < -K::pi) azm += K::two_pi;
        else if (azm > K::pi) azm -= K::two_pi;
        
        float alt = modelAltitude + TheInputMgr->GetMouseDeltaY();
        if (alt < -1.45F) alt = -1.45F;
        else if (alt > 1.45F) alt = 1.45F;
        
        modelAzimuth = azm;
        modelAltitude = alt;
        
        // Update the timer. If we have reach 1/30th of a second, send a message
        // to the server to let it know the new orientation.
        orientationMessageTimer += TheTimeMgr->GetDeltaTime();
        if (orientationMessageTimer >= 33)
        {
			//send the orientation to the server
            TheMessageMgr->SendMessage(kPlayerServer, ClientOrientationMessage(modelAzimuth));
            orientationMessageTimer = 0;
        }
    }
	if (TheMessageMgr->Server())
	{
		TheEntityManager->EnsureEnemies(); 

		// If the player is moving (jumping not included) use fuel
		if (jumping == false && !(inputVector.x == 0 && inputVector.y == 0)){
			float deltaT = TheTimeMgr->GetDeltaTime();
			float currentFuel = getFuel();
			currentFuel = currentFuel - deltaT*fuelModifier;

			if (currentFuel < 0) // Can't have negative fuel
			{
				currentFuel = 0;
			}
			setFuel(currentFuel);
		}

		// Update the timer, send a message to the server about the players information
		clientInformationMessageTimer += TheTimeMgr->GetDeltaTime();
		if (clientInformationMessageTimer >= 100)
		{
			float health = getHealth();
			float fuel = getFuel();
			float scraps = getScraps();

			//send update message about the players info
			TheMessageMgr->SendMessageAll(PlayerInformationMessage(this->GetControllerIndex(),health, fuel, scraps));
			clientInformationMessageTimer = 0;
		}
	}
	SetPosition(this->GetTargetNode()->GetNodePosition());

	//forward vector
	forwardVector = this->GetTargetNode()->GetNodeTransform()[0];
	
	//right vector
	rightVector = this->GetTargetNode()->GetNodeTransform()[1];

	//rotate the player
    SetPlayerOrientation(modelAzimuth);

	if (getFuel() != 0 && !Crouching) //If the player still has fuel
	{
		// move player
		Vector3D vec = this->GetLinearVelocity();
		Vector3D movementVector = Vector3D(0, 0, vec.z);
		Vector2D tempInput= inputVector;
		if (abs(tempInput.x)>0 || abs(tempInput.y) > 0)
			tempInput.Normalize();

		if (GetGroundContact())
			this->SetLinearVelocity((movementVector + (forwardVector*tempInput.x*moveMagnitude) + (rightVector*tempInput.y*moveMagnitude)));
	}


	if (newAnimation != oldAnimation)
	{
		SetPlayerMotion(newAnimation, oldAnimation);
		oldAnimation = newAnimation;
	}

	GetTargetNode()->Animate();

	//Destroy player if they fall off the map
	Point3D temp = GetTargetNode()->GetNodePosition();
	if (temp.z < -250)
		ThePlayerManager->RemovePlayer(GetControllerIndex());

	//if they are self destructing count down the timer
	if (selfDestructActive)
	{
		alarmSoundTimer += TheTimeMgr->GetDeltaTime();
		if (alarmSoundTimer >= 1000)
		{
			Point3D position = GetTargetNode()->GetNodePosition();
			SoundMessage sMessage(kMessageSound, kSelfDestructAlarm, 100.0f, position);
			TheMessageMgr->SendMessageAll(sMessage);
			alarmSoundTimer = 0;
		}

		selfDestructTimer += TheTimeMgr->GetDeltaTime();
		if (selfDestructTimer > 6000){
			for (int i = 0; i < ThePlayerManager->EntityList.size(); i++)
			{
				EntityBase *temp = ThePlayerManager->EntityList.at(i);
				if (temp->getType() == kEntityPlayer)
				{
					MyPlayerController *player = static_cast<MyPlayerController *>(temp);
					Point3D playerPos = player->GetTargetNode()->GetNodePosition();
					if (player->GetControllerIndex() != GetControllerIndex() && Magnitude(playerPos - (GetTargetNode()->GetNodePosition())) < 15)
					{
						PlayerKillRequestMessage message(kPlayerKillRequestMessage, kEntityPlayer, player->GetControllerIndex());
						TheMessageMgr->SendMessage(kPlayerServer, message);
					}
				}
				else if (temp->getType() == kEntityEnemy)
				{
					MyAIController *enemy = static_cast<MyAIController *>(temp);
					Point3D AIPos = enemy->GetTargetNode()->GetNodePosition();
					if (enemy->GetControllerIndex() != GetControllerIndex() && Magnitude(AIPos - (GetTargetNode()->GetNodePosition())) < 15)
					{
						PlayerKillRequestMessage message(kPlayerKillRequestMessage, kEntityEnemy, enemy->GetControllerIndex());
						TheMessageMgr->SendMessage(kPlayerServer, message);
					}
				}

			}
			PlayerKillRequestMessage message(kPlayerKillRequestMessage, kEntityPlayer, GetControllerIndex());

			Point3D position = GetTargetNode()->GetNodePosition();
			SoundMessage sMessage(kMessageSound, kExplosionSound, 100.0f, position);
			TheMessageMgr->SendMessageAll(sMessage);

			TheMessageMgr->SendMessage(kPlayerServer, message);

			selfDestructActive = false;
		}

	}
}

void MyPlayerController::SetMotionFlag(float x, float y)
{
	if (x == 0 && y > 0)
		newAnimation = kLeftAnimation;
	else if (x == 0 && y < 0)
		newAnimation = kRightAnimation;
	else if (x > 0)
		newAnimation = kForwardAnimation;
	else if (x < 0)
		newAnimation = kBackwardAnimation;
}

void MyPlayerController::SetPlayerMotion(animationFlag newAni, animationFlag oldAni)
{
	switch (newAni)
	{
	case kForwardAnimation:
		mainFrameAnimator.SetAnimation("Models/Player/PlayerWalk");
		secondFrameAnimator.SetAnimation(DirectoryConverter(oldAni));
		armFrameAnimator.SetAnimation("Models/Player/PlayerWalk");

		mainFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		armFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);

		mainWeightInterpolator->SetMode(kInterpolatorForward);
		secondWeightInterpolator->SetMode(kInterpolatorBackward);
		armWeightInterpolator->SetMode(kInterpolatorForward);

		mainFrameInterpolator->SetRate(2.2);
		armFrameInterpolator->SetRate(2.2);

		mainFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
		secondFrameInterpolator->SetMode(kInterpolatorForward);
		armFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
		
		break;

	case kBackwardAnimation:
		mainFrameAnimator.SetAnimation("Models/Player/PlayerWalk");
		secondFrameAnimator.SetAnimation(DirectoryConverter(oldAni));
		armFrameAnimator.SetAnimation("Models/Player/PlayerWalk");

		mainFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		armFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);

		mainWeightInterpolator->SetMode(kInterpolatorForward);
		secondWeightInterpolator->SetMode(kInterpolatorBackward);
		armWeightInterpolator->SetMode(kInterpolatorForward);

		mainFrameInterpolator->SetRate(2.2);
		armFrameInterpolator->SetRate(2.2);

		mainFrameInterpolator->SetMode(kInterpolatorBackward | kInterpolatorLoop);
		secondFrameInterpolator->SetMode(kInterpolatorForward);
		armFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);

		break;

	case kLeftAnimation:
		mainFrameAnimator.SetAnimation("Models/Player/PlayerLeft");
		secondFrameAnimator.SetAnimation(DirectoryConverter(oldAni));
		armFrameAnimator.SetAnimation("Models/Player/PlayerLeft");

		mainFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		armFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);

		mainWeightInterpolator->SetMode(kInterpolatorForward);
		secondWeightInterpolator->SetMode(kInterpolatorBackward);
		armWeightInterpolator->SetMode(kInterpolatorForward);

		mainFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
		secondFrameInterpolator->SetMode(kInterpolatorForward);
		armFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);

		break;

	case kRightAnimation:
		mainFrameAnimator.SetAnimation("Models/Player/PlayerRight");
		secondFrameAnimator.SetAnimation(DirectoryConverter(oldAni));
		armFrameAnimator.SetAnimation("Models/Player/PlayerLeft");

		mainFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		armFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);

		mainWeightInterpolator->SetMode(kInterpolatorForward);
		secondWeightInterpolator->SetMode(kInterpolatorBackward);
		armWeightInterpolator->SetMode(kInterpolatorForward);

		mainFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
		secondFrameInterpolator->SetMode(kInterpolatorForward);
		armFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);

		break;

	case kIdleAnimation:
		mainFrameAnimator.SetAnimation("Models/Player/PlayerIdle");
		secondFrameAnimator.SetAnimation(DirectoryConverter(oldAni));
		armFrameAnimator.SetAnimation("Models/Player/PlayerIdle");

		mainFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		armFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);

		mainWeightInterpolator->SetMode(kInterpolatorForward);
		secondWeightInterpolator->SetMode(kInterpolatorBackward);
		armWeightInterpolator->SetMode(kInterpolatorForward);

		mainFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);
		secondFrameInterpolator->SetMode(kInterpolatorForward);
		armFrameInterpolator->SetMode(kInterpolatorForward | kInterpolatorLoop);

		break;

	case kAttackAnimation:
		armFrameAnimator.SetAnimation("Models/Player/PlayerAttack");

		armFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);

		armFrameInterpolator->SetMode(kInterpolatorForward);
		
		break;

	case kChainsawAnimation:
		armFrameAnimator.SetAnimation("Models/Player/PlayerChainsaw");

		armFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);

		armFrameInterpolator->SetMode(kInterpolatorForward);

		break;

	case kCrouchAnimation:
		mainFrameAnimator.SetAnimation("Models/Player/PlayerCrouch");
		secondFrameAnimator.SetAnimation(DirectoryConverter(oldAni));
		armFrameAnimator.SetAnimation("Models/Player/PlayerCrouch");

		mainFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		mainFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetValue(1.0f);
		secondFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);
		armFrameAnimator.GetWeightInterpolator()->SetValue(0.0f);
		armFrameAnimator.GetWeightInterpolator()->SetRate(1.0f / 250.0f);

		mainWeightInterpolator->SetMode(kInterpolatorForward);
		secondWeightInterpolator->SetMode(kInterpolatorBackward);
		armWeightInterpolator->SetMode(kInterpolatorForward);

		mainFrameInterpolator->SetRate(2.5);

		mainFrameInterpolator->SetMode(kInterpolatorForward);
		secondFrameInterpolator->SetMode(kInterpolatorForward);
		armFrameInterpolator->SetMode(kInterpolatorForward);
		
		break;
	}
	
	//send if local player
	GamePlayer *player = static_cast<GamePlayer*>(TheMessageMgr->GetLocalPlayer());
	if (player->GetController() == this)
	{
		PlayerChangeAnimationStateMessage msg(this->GetControllerIndex(), newAni, oldAni);
		TheMessageMgr->SendMessageAll(msg, false);
	}
}

const char* MyPlayerController::DirectoryConverter(animationFlag flag)
{
	switch (flag)
	{
	case kForwardAnimation:
		return "Models/Player/playerWalk";
		break;
	case kBackwardAnimation:
		return "Models/Player/playerWalk";
		break;
	case kLeftAnimation:
		return "Models/Player/playerWalk";
		break;
	case kRightAnimation:
		return "Models/Player/playerWalk";
		break;
	case kIdleAnimation:
		return "Models/Player/playerIdle";
		break;
	case kCrouchAnimation:
		return "Models/Player/playerCrouch";
		break;
	case kAttackAnimation:
		return "Models/Player/playerAttack";
		break;
	case kChainsawAnimation:
		return "Models/Player/playerChainsaw";
		break;
	}
	
}

void MyPlayerController::AnimationCallback(FrameAnimator *frame, CueType cue)
{	
	if (cue == 'Stnd')
	{
		Crouching = false;
	}
}

//Rotate the player
void MyPlayerController::SetPlayerOrientation(float azimuth, float roll)
{
    Matrix3D	rotation;
    rotation.SetRotationAboutZ(azimuth);
    this->SetRigidBodyMatrix3D(Transform(GetTargetNode()->GetSuperNode()->GetInverseWorldTransform(), rotation));
    this->GetFirstShape()->SetNodeTransform(TransposeTransform(rotation, GetFirstShape()->GetNodeTransform()));   
}

//on collision
RigidBodyStatus MyPlayerController::HandleNewGeometryContact(const GeometryContact *contact)
{
	//if with ground
	jumping = false;
	RigidBodyStatus temp = RigidBodyStatus();
	return temp;
}

void MyPlayerController::ReceiveMessage(const ControllerMessage *message)
{
	switch (message->GetControllerMessageType())
	{
		case kNodeMessageState:
		{
			const PlayerStateMessage *msg = static_cast<const PlayerStateMessage *>(message);
			GetTargetNode()->SetNodePosition(msg->GetPosition());
			break;
		}
		case kPlayerMessageDestroy:
		{		
			Destroy();
			break;
		}
		case kPlayerMessageBeginMovement:
		{
			const PlayerMovementMessage *m = static_cast<const PlayerMovementMessage *>(message);
			unsigned_int32 flag = m->GetMovementFlag();
			MyPlayerController * player = static_cast<MyPlayerController*>(TheWorldMgr->GetWorld()->GetController(m->GetControllerIndex()));	
			player->SetMovementFlag(flag);
			break;
		}
		case kPlayerMessageEndMovement:
		{
			const PlayerMovementMessage *m = static_cast<const PlayerMovementMessage *>(message);
			unsigned_int32 flag = m->GetMovementFlag();
			SetMovementFlag(~flag);
			break;
		}
		case kPlayerMessageOrientation:
		{
			const PlayerOrientationMessage *mes = static_cast<const PlayerOrientationMessage *>(message);
			MyPlayerController * player = static_cast<MyPlayerController*>(TheWorldMgr->GetWorld()->GetController(mes->GetControllerIndex()));
			if (player == this)
			{
                modelAzimuth = mes->GetOrientation();
				orientation = mes->GetOrientation(); 
				player->SetOrientation(orientation);
			}
			break;
		}
		case kPlayerInformationMessage:
		{
			const PlayerInformationMessage *mes = static_cast<const PlayerInformationMessage *>(message);
			MyPlayerController * player = static_cast<MyPlayerController*>(TheWorldMgr->GetWorld()->GetController(mes->GetControllerIndex()));
			if (player == this)
			{
				player->setHealth(mes->GetHealth());
				player->setFuel(mes->GetFuel());
				player->setScraps(mes->GetScraps());
			}
			break;
		}
		case kPlayerChangeAnimationStateMessage:
		{
			const PlayerChangeAnimationStateMessage * newanimationmsg = static_cast<const PlayerChangeAnimationStateMessage *>(message);
			SetPlayerMotion(static_cast<animationFlag>(newanimationmsg->GetNewAnimationState()), static_cast<animationFlag>(newanimationmsg->GetOldAnimationState()));
			break;
		}
	}
	RigidBodyController::ReceiveMessage(message);
}

ControllerMessage *MyPlayerController::ConstructMessage(ControllerMessageType type) const
{
	switch (type)
	{
		case kNodeMessageState:
				return (new PlayerStateMessage(GetControllerIndex()));
		case kPlayerMessageBeginMovement:
		case kPlayerMessageEndMovement:

			return (new PlayerMovementMessage(type, GetControllerIndex()));
		case kPlayerChangeAnimationStateMessage:
			return (new PlayerChangeAnimationStateMessage(GetControllerIndex()));
		case kPlayerMessageDestroy:
		{
			return (new PlayerDestroyMessage(GetControllerIndex()));
		}
		case kPlayerMessageOrientation:
		{
			return (new PlayerOrientationMessage(GetControllerIndex(), orientation));
		}
		case kPlayerInformationMessage:
		{
			return (new PlayerInformationMessage(GetControllerIndex(), mHealth, mFuel, mScraps));
		}
	
	}

	return (RigidBodyController::ConstructMessage(type));
}

//Player gets destroyed
void MyPlayerController::Destroy()
{
	Node *node = this->GetTargetNode();
	if (node)
	{
		delete node;
	}
}

void MyPlayerController::BeginMovement(unsigned_int32 flag)
{
	const Point3D& position = GetTargetNode()->GetWorldPosition();
	Vector3D velocity = GetLinearVelocity();
	float fuel = getFuel();
	PlayerMovementMessage message(kPlayerMessageBeginMovement, GetControllerIndex(), position, velocity, flag);
	TheMessageMgr->SendMessageAll(message);
}

void MyPlayerController::EndMovement(unsigned_int32 flag)
{
	const Point3D& position = GetTargetNode()->GetWorldPosition();
	Vector3D velocity = GetLinearVelocity();
	float fuel = getFuel();
	PlayerMovementMessage message(kPlayerMessageEndMovement, GetControllerIndex(), position, velocity, flag);
	TheMessageMgr->SendMessageAll(message);
}

void MyPlayerController::BeginOrientation(float _orientation, Player *sender)
{
	PlayerOrientationMessage message(GetControllerIndex(), _orientation);
	TheMessageMgr->SendMessageClients(message, sender);
	TheMessageMgr->SendMessage(kPlayerServer, message);
}

//Player's health drops to zero
void MyPlayerController::OnDeath()
{

	EntityPlayer::OnDeath();
	ThePlayerManager->RemovePlayer(GetControllerIndex());
}

//Get the player key associated with this controller
PlayerKey MyPlayerController::getPlayerKey()
{
	GamePlayer *gp = nullptr;
	MyPlayerController *cont = nullptr;
	Node *node = nullptr;
	PlayerKey key = -1;
	long id = -1;
	Point3D loc;
	Player *p = TheMessageMgr->GetFirstPlayer();
	while (p)
	{
		gp = static_cast<GamePlayer *>(p);
		cont = static_cast<MyPlayerController *>(gp->GetController());
		if (cont->GetControllerIndex() == this->GetControllerIndex()) //If it is the correct controller
		{
			key = gp->GetPlayerKey();
			return key;
		}
		p = p->Next();
	}
	return NULL;
}

void MyPlayerController::SetSelfDestruct(){
	selfDestructActive = !selfDestructActive;
	alarmSoundTimer = -1000;
	if (selfDestructActive)
	{
		Point3D position = GetTargetNode()->GetNodePosition();
		SoundMessage sMessage(kMessageSound, kSelfDestructInitiated, 100.0f, position);
		TheMessageMgr->SendMessageAll(sMessage);
	}
}
