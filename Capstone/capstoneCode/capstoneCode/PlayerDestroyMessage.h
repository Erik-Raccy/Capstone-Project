#pragma once

/*
* This message is constructed when a player times out in HandlePlayerEvent
* and is used to remove their player from the world on all connected computers
*/

#include "MyMessages.h"

namespace C4
{
	class PlayerDestroyMessage : public ControllerMessage
	{
	public:
		//Constructors
		PlayerDestroyMessage(long controllerIndex);
		~PlayerDestroyMessage();
	};
}