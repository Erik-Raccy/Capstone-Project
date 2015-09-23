#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class RemoveArmorAndWeaponsFromInventoryMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		RemoveArmorAndWeaponsFromInventoryMessage(MessageType type, long contIndex, unsigned_int32 _type);
		RemoveArmorAndWeaponsFromInventoryMessage(MessageType type);
		~RemoveArmorAndWeaponsFromInventoryMessage();

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

