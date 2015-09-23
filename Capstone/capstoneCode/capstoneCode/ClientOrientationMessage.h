#pragma once

/*
* This message is constructed by client users to send orientation mouse inputs
* to the server (which may sometimes be itself) created and sent from the character controller
* when recieved by the server, its handlemessage function is called
*/

#include "MyMessages.h"

namespace C4
{

	class ClientOrientationMessage : public Message
	{
	private:

		float orientation;

	public:
		ClientOrientationMessage(void);
		ClientOrientationMessage(float _orientation);
		~ClientOrientationMessage(void);

		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);

		bool HandleMessage(Player *sender) const;
	};



}
