#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class EntityRemoveMessage : public Message
	{
		friend class Game;

	public:
		//Constructor
		EntityRemoveMessage(MessageType type,  long contIndex);
		EntityRemoveMessage(MessageType type);
		~EntityRemoveMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;


	private:
		//Variables
		long controllerIndex;

	};
}

