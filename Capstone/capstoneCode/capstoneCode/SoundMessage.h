#pragma once

#include "MyMessages.h"
#include "Game.h"
#include "C4Engine.h"

namespace C4{

	//Enum for the different sounds
	enum SoundEffect
	{
		kChainsawSound,
		kBlenderSound,
		kPunchSound,
		kRocketSound,
		kAIDeathSound,
		kAIChaseSound,
		kHealthLowSound,
		kPickupSound,
		kEquipSound,
		kZipperSound,
		kJumpSound,
		kDropItemSound,
		kWalkSound,
		kComeBackSound,
		kSelfDestructInitiated,
		kSelfDestructAlarm,
		kExplosionSound,
		kCommunistDeath,
		kCommunistDetected
	};

	class SoundMessage : public Message
	{
		friend class Game;

	public:
		//Constructors
		SoundMessage(MessageType type, unsigned_int32 soundType, float soundDistance, Point3D position);
		SoundMessage(MessageType type);
		~SoundMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		//Variables
		Point3D soundPos;
		float soundDist;
		unsigned_int32 soundType;
	};
}

