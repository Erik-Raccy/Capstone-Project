#include "AddArmorAndWeaponToPlayersMessage.h"

using namespace C4;


AddArmorAndWeaponToPlayersMessage::AddArmorAndWeaponToPlayersMessage(long contIndex, unsigned_int32 weaponType, unsigned_int32 armorType) : Message(kAddArmorAndWeaponMessage)
{
	equippedWeapon = weaponType;
	equippedArmor= armorType;
	controllerIndex = contIndex;
}

AddArmorAndWeaponToPlayersMessage::AddArmorAndWeaponToPlayersMessage() : Message(kAddArmorAndWeaponMessage)
{

}
AddArmorAndWeaponToPlayersMessage::~AddArmorAndWeaponToPlayersMessage()
{
}

void AddArmorAndWeaponToPlayersMessage::Compress(Compressor& data) const
{
	data << equippedWeapon;
	data << equippedArmor;
	data << controllerIndex;
}

bool AddArmorAndWeaponToPlayersMessage::Decompress(Decompressor& data)
{
	int temp;
	data >> equippedWeapon;
	data >> equippedArmor;
	data >> temp;

	controllerIndex = temp;
	return (true);
}
//Adds the correct armor and weapon to the other players so that the 
//Player who is spawning sees them all correctly
bool AddArmorAndWeaponToPlayersMessage::HandleMessage(Player *sender) const
{
	MyWorld *world = static_cast<MyWorld *>(TheWorldMgr->GetWorld());
	MyPlayerController *spawner = static_cast<MyPlayerController*>(TheWorldMgr->GetWorld()->GetController(controllerIndex));
	Model *model = spawner->GetTargetNode();
	
	Geometry *geom = static_cast<Geometry*>(model->FindNode("LibertyPrime"));
	MaterialObject *mat = geom->GetMaterialObject(0)->Clone();
	MaterialObject *mat2 = geom->GetMaterialObject(0)->Clone();

	TextureMapAttribute* texAttribute = static_cast<TextureMapAttribute*>(mat->FindAttribute(kAttributeTextureMap));
	TextureMapAttribute* texAttribute2 = static_cast<TextureMapAttribute*>(mat2->FindAttribute(kAttributeTextureMap));


	//default
	//Upper texture = lower body
	// leg texture = upper body

	char* upperTextureName = "textures/legsRustMaterial";
	char* legTextureName = "textures/upperRustMaterial";
	float defense = ARMOR_RUST_DEFENSE;

	//Find the correct texture
	switch (equippedArmor)
	{
	case k_ArmorIron:
		upperTextureName = "textures/legsIronMaterial";
		legTextureName = "textures/upperIronMaterial";
		defense = ARMOR_IRON_DEFENSE;
		break;
	case k_ArmorBronze:
		upperTextureName = "textures/legsBronzeMaterial";
		legTextureName = "textures/upperBronzeMaterial";
		defense = ARMOR_BRONZE_DEFENSE;
		break;
	case k_ArmorSteel:

		upperTextureName = "textures/legsSteelMaterial";
		legTextureName = "textures/upperSteelMaterial";
		defense = ARMOR_STEEL_DEFENSE;
		break;
	case k_ArmorGold:

		upperTextureName = "textures/legsGoldMaterial";
		legTextureName = "textures/upperGoldMaterial";
		defense = ARMOR_GOLD_DEFENSE;
		break;
	case k_ArmorDiamond:
		upperTextureName = "textures/legsDiamondMaterial";
		legTextureName = "textures/upperDiamondMaterial";
		defense = ARMOR_DIAMOND_DEFENSE;
		break;
	case k_ArmorTitanium:

		upperTextureName = "textures/legsTitaniumMaterial";
		legTextureName = "textures/upperTitaniumMaterial";
		defense = ARMOR_TITANIUM_DEFENSE;
		break;
	}
	//Set the correct texture of the player
	EntityArmor *armor = new EntityArmor(kEntityArmor, defense, Point3D(NULL, NULL, NULL), nullptr, (EquipmentType)equippedArmor);
	spawner->setArmor(armor);


	//Change the texture
	texAttribute->SetTexture(legTextureName);
	geom->SetMaterialObject(0, mat);
	mat->Release();
	texAttribute2->SetTexture(upperTextureName);
	geom->SetMaterialObject(1, mat2);

	geom->InvalidateShaderData();
	mat2->Release();
	//Get correct model of the weapon
	unsigned_int32 modelID = NULL;
	float damage = WEAPON_NO_WEAPON_DAMAGE;
	float knockback = 1;
	float range = 1;

	//Find the correct weapon the player has
	switch (equippedWeapon)
	{
	case k_weaponBlender:
		modelID = kModelBlender;
		damage = WEAPON_BLENDER_DAMAGE;
		knockback = WEAPON_BLENDER_KNOCKBACK;
		range = WEAPON_BLENDER_RANGE;
		break;
	case k_weaponChainsaw:
		modelID = kModelChainsaw;
		damage = WEAPON_CHAINSAW_DAMAGE;
		knockback = WEAPON_CHAINSAW_KNOCKBACK;
		range = WEAPON_CHAINSAW_RANGE;
		break;
	case k_weaponGun:
		modelID = kModelRocketLauncher;
		damage = WEAPON_ROCKETLAUNCHER_DAMAGE;
		knockback = WEAPON_ROCKETLAUNCHER_KNOCKBACK;
		range = WEAPON_ROCKETLAUNCHER_RANGE;
		break;
	case k_weaponSelfDestructGun:
		modelID = kModelClockLauncher;
		damage = WEAPON_ROCKETLAUNCHER_DAMAGE;
		knockback = WEAPON_ROCKETLAUNCHER_KNOCKBACK;
		range = WEAPON_ROCKETLAUNCHER_RANGE;
		break;
	}

	//Add that model to the player
	if (modelID)
	{
		Model *chainsaw = Model::Get(modelID);
		Point3D weaponPosition = model->GetWorldPosition();
		world->AddNewNode(chainsaw); // add the weapon model
		model->FindNode("Weapon")->AddNewSubnode(chainsaw);
		EntityWeapon *weapon = new EntityWeapon(kEntityWeapon, Point3D(NULL, NULL, NULL), damage, knockback, range, nullptr, (EquipmentType)equippedWeapon);
		spawner->setWeapon(weapon);
	}

	return (true);
}
