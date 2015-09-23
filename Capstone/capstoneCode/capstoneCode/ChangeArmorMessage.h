#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class ChangeArmorMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		ChangeArmorMessage(MessageType type, unsigned_int32 _type, long contIndex);
		ChangeArmorMessage(MessageType type);
		~ChangeArmorMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		//Methods
		unsigned_int32 equipped;
		long controllerIndex;

	};
}

