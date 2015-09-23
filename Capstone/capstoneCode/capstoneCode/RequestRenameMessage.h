#pragma once

#include "MyMessages.h"

namespace C4
{

	class RequestRenameMessage : public Message
	{
	public:
		//constructors
		RequestRenameMessage();
		RequestRenameMessage(const char * name);
		~RequestRenameMessage();

		// Constructors for child classes
		RequestRenameMessage(MessageType type);
		RequestRenameMessage(MessageType type, const char *name);

		//accessor
		const char * GetNewName(void) const
		{
			return newName;
		}

		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);

	private:
		String<kMaxPlayerNameLength> newName;
	};


	class NameChangeMessage : public RequestRenameMessage
	{
	private:
		PlayerKey playerKey; //player's ID who we're talking aboot
	public:
		//constructors
		NameChangeMessage();
		NameChangeMessage(const char * name, PlayerKey pk);
		~NameChangeMessage();

		// The compress and decompress methods are obligatory.
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);

		// Simple accessor for our player key value
		const PlayerKey GetPlayerKey() const {
			return playerKey;
		}
	};


}