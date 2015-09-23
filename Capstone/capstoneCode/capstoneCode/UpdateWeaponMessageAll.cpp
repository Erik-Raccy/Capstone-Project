#include "UpdateWeaponMessageAll.h"

using namespace C4;


UpdateWeaponMessageAll::UpdateWeaponMessageAll(MessageType type, long contIndex, unsigned_int32 equipmentType) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

UpdateWeaponMessageAll::UpdateWeaponMessageAll(MessageType type) : Message(type)
{

}
UpdateWeaponMessageAll::~UpdateWeaponMessageAll()
{
}

void UpdateWeaponMessageAll::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex;
}

bool UpdateWeaponMessageAll::Decompress(Decompressor& data)
{
	int temp;
	data >> equipped;
	data >> temp;

	controllerIndex = temp;
	return (true);
}

//Change the weapon model of a player
bool UpdateWeaponMessageAll::HandleMessage(Player *sender) const
{
	Controller *cont = nullptr;
	Node *node = nullptr;
	Player *p = TheMessageMgr->GetFirstPlayer();
	while (p)
	{
		GamePlayer *gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			node = cont->GetTargetNode();
			long  id = cont->GetControllerIndex();

			if (id == controllerIndex){ // If this is the player that changed weapons

				MyWorld *world = static_cast<MyWorld *>(TheWorldMgr->GetWorld());
				MyPlayerController *player = static_cast<MyPlayerController *>(gp->GetController());

				//Remove previous weapon
				Model *model = player->GetTargetNode();
				Node *n = model->FindNode("Weapon")->GetFirstSubnode();
				if (n)
				{
					delete n;
				}
							
				//Default 
				unsigned_int32 modelID = NULL;
				float damage = WEAPON_NO_WEAPON_DAMAGE;
				float knockback = WEAPON_NO_WEAPON_KNOCKBACK;
				float range = WEAPON_NO_WEAPON_RANGE;

				//Find the correct model to attach
				switch (equipped)
				{
				case k_weaponBlender:
					damage = WEAPON_BLENDER_DAMAGE;
					knockback = WEAPON_BLENDER_KNOCKBACK;
					range = WEAPON_BLENDER_RANGE;
					modelID = kModelBlender;
					break;
				case k_weaponChainsaw:
					damage = WEAPON_CHAINSAW_DAMAGE;
					knockback = WEAPON_CHAINSAW_KNOCKBACK;
					range = WEAPON_CHAINSAW_RANGE;
					modelID = kModelChainsaw;
					break;
				case k_weaponGun:
					damage = WEAPON_ROCKETLAUNCHER_DAMAGE;
					knockback = WEAPON_ROCKETLAUNCHER_KNOCKBACK;
					range = WEAPON_ROCKETLAUNCHER_RANGE;
					modelID = kModelRocketLauncher;
					break;
				case k_weaponSelfDestructGun:
					damage = SELF_DESTRUCT_DAMAGE;
					knockback = SELF_DESTRUCT_DAMAGE;
					range = SELF_DESTRUCT_DAMAGE;
					modelID = kModelClockLauncher;
					break;
				}

				if (modelID)
				{
					//Attach the model to the player
					Model *chainsaw = Model::Get(modelID);
					Point3D weaponPosition = model->GetWorldPosition();
					world->AddNewNode(chainsaw);
					model->FindNode("Weapon")->AddNewSubnode(chainsaw);

					//Set the current weapon to be the weapon that was equipped
					EntityWeapon *weapon = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), damage, knockback, range, nullptr, (EquipmentType)equipped);
					player->setWeapon(weapon);
				}
			}
		}
		p = p->Next();
	}
	return (true);
}
