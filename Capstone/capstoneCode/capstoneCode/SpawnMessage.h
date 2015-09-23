#ifndef SPAWNMESSAGE_H
#define SPAWNMESSAGE_H

/*
 * These messages upon connection to spawn players into the world 
 */


#include "MyMessages.h"

namespace C4
{
	class SpawnMessage : public Message
	{
	public:

		SpawnMessage();
		SpawnMessage(PlayerKey playerkey, int32 controllerIndex, Point3D location);
		~SpawnMessage();

		bool HandleMessage(Player *sender) const;

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);

		Point3D GetLocation() const
		{
			return loc;
		}

		PlayerKey GetPlayerKey() const
		{
			return key;
		}

		long GetControllerIndex() const
		{
			return contIndex;
		}
	private:

		Point3D loc;
		int32 contIndex;
		PlayerKey key;
	};
	
	//debuging code for drops
	class SpawnDropMessage : public Message
	{
	public:

		SpawnDropMessage();
		SpawnDropMessage(int32 controllerIndex, Point3D location, int32 entity, int32 subType);
		~SpawnDropMessage();

		bool HandleMessage(Player *sender) const;

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);

		Point3D GetLocation() const
		{
			return loc;
		}

		long GetControllerIndex() const
		{
			return contIndex;
		}
	private:
		Point3D loc;
		int32 contIndex;
		int32 entityType;
		int32 subType;
	};

	class SpawnAIMessage : public Message
	{
	public:

		SpawnAIMessage();
		SpawnAIMessage(int32 controllerIndex, Point3D location);
		~SpawnAIMessage();

		bool HandleMessage(Player *sender) const;

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);
		
		Point3D GetLocation() const
		{
			return loc;
		}

		long GetControllerIndex() const
		{
			return contIndex;
		}
	private:

		Point3D loc;
		int32 contIndex;
	};

}


#endif 