#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{

	class SpawnParticleMessage : public Message
	{
		friend class Game;

	public:
		//Accessors
		SpawnParticleMessage(Point3D position, int numOfSparks);
		SpawnParticleMessage(Point3D position, int numOfSparks,int noUse);
		SpawnParticleMessage();
		~SpawnParticleMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		Point3D position;
		int numOfSparks;
		int noUse;
	};
}

