#pragma once

/*
 * This message is constructed by client users to send movement keys actions
 * to the server (which may sometimes be itself) created and sent from action class
 * when recieved by the server, its handlemessage function is called
 */

#include "MyMessages.h"

namespace C4
{
	class ClientMovementMessage : public Message
	{
		friend class Game;

	private:
		unsigned_int32	movementFlag;
		ClientMovementMessage(MessageType type);

	public:
		ClientMovementMessage(MessageType type, unsigned_int32 flag);
		~ClientMovementMessage();

		unsigned_int32 GetMovementFlag(void) const
		{
			return (movementFlag);
		}

		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
		bool HandleMessage(Player *sender) const;
	};


}
