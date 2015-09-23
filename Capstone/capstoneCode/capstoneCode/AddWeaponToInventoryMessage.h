#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class AddWeaponToInventoryMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		AddWeaponToInventoryMessage(MessageType type, long contIndex, unsigned_int32 _type);
		AddWeaponToInventoryMessage(MessageType type);
		~AddWeaponToInventoryMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		//Variables
		unsigned_int32 equipped;
		long controllerIndex;


	};
}

