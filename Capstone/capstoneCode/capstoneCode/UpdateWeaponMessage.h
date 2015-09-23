#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class UpdateWeaponMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		UpdateWeaponMessage(MessageType type, unsigned_int32 _type, long contIndex);
		UpdateWeaponMessage(MessageType type);
		~UpdateWeaponMessage();

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

