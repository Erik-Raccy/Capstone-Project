#include "SoundMessage.h"

using namespace C4;


SoundMessage::SoundMessage(MessageType type, unsigned_int32 sType, float distance, Point3D pos) : Message(type)
{
	soundType = sType;
	soundDist = distance;
	soundPos = pos;
}

SoundMessage::SoundMessage(MessageType type) : Message(type)
{

}
SoundMessage::~SoundMessage()
{
}

void SoundMessage::Compress(Compressor& data) const
{
	data << soundType;
	data << soundDist;
	data << soundPos;
}

bool SoundMessage::Decompress(Decompressor& data)
{
	data >> soundType;
	data >> soundDist;
	data >> soundPos;

	return (true);
}

//Plays the sound that is soundType, at soundPosition and can be heard soundDist away
bool SoundMessage::HandleMessage(Player *sender) const
{
	Player *player = TheMessageMgr->GetLocalPlayer();
	if (player) //If local player
	{
		World *world = TheWorldMgr->GetWorld();
		MyPlayerController *controller = static_cast<MyPlayerController *>(static_cast<GamePlayer *>(player)->GetController());
		
		int mult = 1; //To make the sound louder

		//File sound is located 
		char* sound; 

		//Find the correct location of the sound
		switch (soundType)
		{
		case kChainsawSound:
			sound = "Sounds/Chainsaw";
			break;
		case kBlenderSound:
			sound = "Sounds/Blender";
			break;
		case kPunchSound:
			sound = "Sounds/Punch";
			break;
		case kRocketSound:
			sound = "Sounds/RocketLauncher";
			break;
		case kAIDeathSound:
			sound = "Sounds/Malfunction";
			break;
		case kAIChaseSound:
			sound = "Sounds/DestroyYou";
			mult = 15;
			break;
		case kHealthLowSound:
			sound = "Sounds/HealthLow";
			break;
		case kComeBackSound:
			sound = "Sounds/ComeBack";
			break;
		case kPickupSound:
			sound = "Sounds/Pickup";
			break; 
		case kEquipSound:
			sound = "Sounds/Equip";
			break;
		case kZipperSound:
			sound = "Sounds/Zipper";
			break;
		case kDropItemSound:
			sound = "Sounds/DropItem";
			break;
		case kJumpSound:
			sound = "Sounds/Jump";
			break;
		case kWalkSound:
			sound = "Sounds/walkMedium";
			break;
		case kSelfDestructInitiated:
			sound = "Sounds/destructionInitiated";
			break;
		case kSelfDestructAlarm:
			sound = "Sounds/destructionBeep";
			break;
		case kExplosionSound:
			sound = "Sounds/explosion";
			break;
		case kCommunistDeath:
			sound = "Sounds/deathCommunism";
			break;
		case kCommunistDetected:
			sound = "Sounds/communistDetected";
			break;
		default:
			sound = "Sounds/Punch";
			break;
		}
		// Create a source for the sound
		OmniSource *source = new OmniSource(sound, soundDist);
		source->SetNodePosition(soundPos);

		//Add source to world and play the sound
		world->AddNewNode(source);
		source->SetSourceVolume(5.0f*mult);
		source->Play();
	}
	return (true);
}
