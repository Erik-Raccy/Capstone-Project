#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class ChangeArmorMessageAll : public Message
	{
		friend class Game;

	public:
		//Constructor
		ChangeArmorMessageAll(MessageType type, long contIndex, unsigned_int32 _type);
		ChangeArmorMessageAll(MessageType type);
		~ChangeArmorMessageAll();

		//Method
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		//Variables
		unsigned_int32 equipped;
		long controllerIndex;
	};
}

