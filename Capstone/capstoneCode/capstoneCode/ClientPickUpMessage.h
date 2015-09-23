#pragma once

#include "MyMessages.h"

namespace C4
{
	class ClientPickUpMessage : public Message
	{
		friend class Game;

	private:
		ClientPickUpMessage(void);
	public:

		ClientPickUpMessage(MessageType type, long controllerIndex, unsigned_int32 equipmentType);
		ClientPickUpMessage(MessageType type);
		~ClientPickUpMessage(void);

		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;

	private:
		unsigned_int32 equipType;
		long controllerIndex;
	};


}