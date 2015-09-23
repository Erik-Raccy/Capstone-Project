#include "MyAttackAction.h"

using namespace C4;

MyAttackAction::MyAttackAction(unsigned_int32 type) : Action(type)
{
	attackType = type;
}

MyAttackAction::~MyAttackAction()
{
}

// If the attack button was pressewd
void MyAttackAction::Begin(void)
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());

		// If it is the local player and they have not attacked in the previous half second
		if (controller && (controller->cooldownCounter >= controller->cooldown) && !controller->Crouching) 
		{
			controller->attacking = true;
			controller->cooldownCounter = 0; //reset cooldown 
			//Engine::Report("attacking");
			
			EntityWeapon *wep = controller->getWeapon();
			float dam = wep->getDamage();
			float knock = wep->getKnockback();
			float range = wep->getRange();

			unsigned_int32 weaponType= wep->GetWeaponType();

			if (weaponType == k_weaponGun && controller->getScraps() >= 10) //If the player is holding the rocket launcher and if they have ammo
			{
				controller->newAnimation = kAttackAnimation;

				//send update message about the players info
				TheMessageMgr->SendMessage(kPlayerServer, PlayerInformationMessage(controller->GetControllerIndex(), controller->getHealth(), controller->getFuel(), controller->getScraps()-10));
				FrustumCamera *cam = TheWorldMgr->GetWorld()->GetCamera();

				Model *model = controller->GetTargetNode();
				Point3D playerPosition = model->GetNodePosition();
				Vector3D forwardVector = model->GetNodeTransform()[0];
				Point3D camPos = cam->GetNodePosition();

				Vector3D camToPlayerVector = playerPosition - cam->GetNodePosition();
				Point3D lookAtPoint = Point3D(playerPosition.x + 3 * forwardVector.y, playerPosition.y - 3 * forwardVector.x, playerPosition.z + 2);

				Vector3D shootDir = Normalize(lookAtPoint - cam->GetNodePosition());
				shootDir.z = shootDir.z + 0.2;
				Point3D shotPos = Point3D(playerPosition.x + 2 * shootDir.x, playerPosition.y + 2 * shootDir.y, playerPosition.z + 1.5F);

				//Send the rocket launcher sound to everyone
				Point3D position = controller->GetTargetNode()->GetNodePosition();
				SoundMessage sMessage(kMessageSound, kRocketSound, 100.0f, position);
				TheMessageMgr->SendMessageAll(sMessage);

				//Send message to spawn a new projectile 
				NewProjectileRequestMessage message(weaponType, shotPos, shootDir);
				TheMessageMgr->SendMessage(kPlayerServer, message);
			}
			else if (weaponType == k_weaponSelfDestructGun && controller->getScraps() >= 5)
			{
				//send update message about the players info
				TheMessageMgr->SendMessage(kPlayerServer, PlayerInformationMessage(controller->GetControllerIndex(), controller->getHealth(), controller->getFuel(), controller->getScraps() - 5));
				FrustumCamera *cam = TheWorldMgr->GetWorld()->GetCamera();

				Model *model = controller->GetTargetNode();
				Point3D playerPosition = model->GetNodePosition();
				Vector3D forwardVector = model->GetNodeTransform()[0];
				Point3D camPos = cam->GetNodePosition();

				Vector3D camToPlayerVector = playerPosition - cam->GetNodePosition();
				Point3D lookAtPoint = Point3D(playerPosition.x + 3 * forwardVector.y, playerPosition.y - 3 * forwardVector.x, playerPosition.z + 2);

				Vector3D shootDir = Normalize(lookAtPoint - cam->GetNodePosition());
				shootDir.z = shootDir.z + 0.2;
				Point3D shotPos = Point3D(playerPosition.x + 2.0F * shootDir.x, playerPosition.y + 2.0F * shootDir.y, playerPosition.z + 1.5F);

				//Send the rocket launcher sound to everyone
				Point3D position = controller->GetTargetNode()->GetNodePosition();
				SoundMessage sMessage(kMessageSound, kRocketSound, 100.0f, position);
				TheMessageMgr->SendMessageAll(sMessage);

				//Send message to spawn a new projectile 
				NewProjectileRequestMessage message(weaponType, shotPos, shootDir);
				TheMessageMgr->SendMessage(kPlayerServer, message);
			}

			//If the weapon is not the rocket launcher (and therefore a melee weapon) 
			else
			{
				Point3D position = controller->GetTargetNode()->GetNodePosition();
				unsigned_int32 soundEffect;
				// dep[ending on the weapon equipped get the associated sound
				switch (weaponType)
				{
				case k_weaponBlender:
					soundEffect = kBlenderSound;
					controller->newAnimation = kAttackAnimation;
					controller->cooldown = controller->PUNCHCOOLDOWN;
					break;
				case k_weaponChainsaw:
					soundEffect = kChainsawSound;
					controller->newAnimation = kChainsawAnimation;
					controller->cooldown = controller->CHAINSAWCOOLDOWN;
					break;
				case k_weaponNoWeapon:
					soundEffect = kPunchSound;
					controller->newAnimation = kAttackAnimation;
					controller->cooldown = controller->PUNCHCOOLDOWN;
					break;
				default:
					soundEffect = kPunchSound;
					controller->newAnimation = kAttackAnimation;
					controller->cooldown = controller->PUNCHCOOLDOWN;
					break;
				}
				//Send the weapon sound to everyone
				SoundMessage sMessage(kMessageSound, soundEffect, 100.0f, position);
				TheMessageMgr->SendMessageAll(sMessage);
	
				//Send attack message to server
				AttackMessage message(kMessageAttack, dam, knock);
				TheMessageMgr->SendMessage(kPlayerServer, message); 
			}

			return;
		}
	}
}

void MyAttackAction::End(void)
{

}