#include "ChangeAIStateMessage.h"

using namespace C4;

ChangeAIStateMessage::ChangeAIStateMessage(long contIndex) : ControllerMessage(kChangeStateMessage, contIndex)
{
	state = NULL;
}

ChangeAIStateMessage::ChangeAIStateMessage(long contIndex, int32 state) : ControllerMessage(kChangeStateMessage, contIndex)
{
	this->state = state;
}

ChangeAIStateMessage::~ChangeAIStateMessage()
{
}

void ChangeAIStateMessage::Compress(Compressor &data) const
{
	ControllerMessage::Compress(data);
	data << state;
}

bool ChangeAIStateMessage::Decompress(Decompressor &data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> state;
		return true;
	}
	return false;
}

