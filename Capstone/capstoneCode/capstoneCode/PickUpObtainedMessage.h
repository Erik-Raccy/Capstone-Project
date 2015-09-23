#pragma once

#include "C4Messages.h"
#include "Game.h"

namespace C4
{
	class PickUpObtainedMessage : public ControllerMessage
	{
		friend class PickUpController;

	public:
		//Constructors
		PickUpObtainedMessage(long controllerIndex);
		~PickUpObtainedMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};
}
