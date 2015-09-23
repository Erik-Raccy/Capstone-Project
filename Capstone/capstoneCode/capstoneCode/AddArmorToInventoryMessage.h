#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class AddArmorToInventoryMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		AddArmorToInventoryMessage(MessageType type, long contIndex, unsigned_int32 _type);
		AddArmorToInventoryMessage(MessageType type);
		~AddArmorToInventoryMessage();

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

