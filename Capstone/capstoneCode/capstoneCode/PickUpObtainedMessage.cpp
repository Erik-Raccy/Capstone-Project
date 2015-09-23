#include "PickUpObtainedMessage.h"

using namespace C4;

PickUpObtainedMessage::PickUpObtainedMessage(long controllerIndex) : ControllerMessage(kPickUpObtainedMessage, controllerIndex)
{
}

PickUpObtainedMessage::~PickUpObtainedMessage()
{
}

void PickUpObtainedMessage::Compress(Compressor& data) const
{
	ControllerMessage::Compress(data);

}

bool PickUpObtainedMessage::Decompress(Decompressor& data)
{
	if (ControllerMessage::Decompress(data))
	{
		return (true);
	}

	return (false);
}