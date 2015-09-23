#pragma once

#include "MyMessages.h"

namespace C4{
	class ServerHealthUpdateMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		ServerHealthUpdateMessage(MessageType type, long contIndex, float damage, unsigned_int32 enumID, int32 attackIndex);
		ServerHealthUpdateMessage(MessageType type);
		~ServerHealthUpdateMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;
	
	private:
		//Variables
		float newHealth;
		float damage;
		long controllerIndex;
		int32 attackerIndex;
		unsigned_int32 contollerEnum;
	};
}


