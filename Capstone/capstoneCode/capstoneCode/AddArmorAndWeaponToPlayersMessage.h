#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class AddArmorAndWeaponToPlayersMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		AddArmorAndWeaponToPlayersMessage(long contIndex, unsigned_int32 _weaponType, unsigned_int32 _armorType);
		AddArmorAndWeaponToPlayersMessage();
		~AddArmorAndWeaponToPlayersMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		//Variables
		unsigned_int32 equippedArmor;
		unsigned_int32 equippedWeapon;
		long controllerIndex;


	};
}

