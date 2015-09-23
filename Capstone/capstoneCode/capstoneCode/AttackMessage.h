#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{
	class AttackMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		AttackMessage(MessageType type, float _damage, float _knockback);
		~AttackMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		//Variables
		float damage;
		float knockback;

		//Constructor
		AttackMessage(MessageType type);
	};
}

