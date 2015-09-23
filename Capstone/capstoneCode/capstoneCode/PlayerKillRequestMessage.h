#pragma once

/*
* File:  PlayerKillRequestMessage.h
* --------------------
* This file contains the message used by the client
* to request to kill a player. This message is always 
* sent by the clients.
*/

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4
{
	class PlayerKillRequestMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		PlayerKillRequestMessage(MessageType type);
		PlayerKillRequestMessage(MessageType mType, EntityType eType, unsigned_int32 _killContIndex);
		~PlayerKillRequestMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);

		unsigned_int32 GetKillIndex() const
		{
			return killIndex;
		}
		
		int GetEntityType() const
		{
			return entityType;
		}

	private:
		//Variables
		unsigned_int32 killIndex;
		unsigned_int32 entityType;
	};
}

