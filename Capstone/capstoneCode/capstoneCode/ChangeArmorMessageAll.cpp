#include "ChangeArmorMessageAll.h"

using namespace C4;


ChangeArmorMessageAll::ChangeArmorMessageAll(MessageType type,  long contIndex,unsigned_int32 equipmentType) : Message(type)
{
	equipped = equipmentType;
	controllerIndex = contIndex;
}

ChangeArmorMessageAll::ChangeArmorMessageAll(MessageType type) : Message(type)
{

}
ChangeArmorMessageAll::~ChangeArmorMessageAll()
{
}

void ChangeArmorMessageAll::Compress(Compressor& data) const
{
	data << equipped;
	data << controllerIndex;
}

bool ChangeArmorMessageAll::Decompress(Decompressor& data)
{
	int temp;
	data >> equipped;
	data >> temp;

	controllerIndex = temp;
	return (true);
}

//Change the armor texture of a player
bool ChangeArmorMessageAll::HandleMessage(Player *sender) const
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
			long  id = cont->GetControllerIndex();
			if (id == controllerIndex){ // If this is the player that changed texture

				// Get the robot texture
				MyPlayerController *player = static_cast<MyPlayerController *>(gp->GetController());
				Model *model = player->GetTargetNode();
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
				switch (equipped)
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

				if (player)
				{
					//Set the new armor as the equipped armor
					EntityArmor *armor = new EntityArmor(kEntityArmor, defense, Point3D(NULL, NULL, NULL), nullptr, (EquipmentType)equipped);
					player->setArmor(armor);
				}
				
				//Change the texture
				texAttribute->SetTexture(legTextureName);
				geom->SetMaterialObject(0, mat);
				mat->Release();
				texAttribute2->SetTexture(upperTextureName);
				geom->SetMaterialObject(1, mat2);

				geom->InvalidateShaderData();
				mat2->Release();
			}
		}

		p = p->Next();
	}
	
	return (true);
}
