#include "ChangeAIStateMessage.h"

using namespace C4;

PlayerChangeAnimationStateMessage::PlayerChangeAnimationStateMessage(long contIndex) : ControllerMessage(kPlayerChangeAnimationStateMessage, contIndex)
{
	oldAnimationState = NULL;
	newAnimationState = NULL;
}

PlayerChangeAnimationStateMessage::PlayerChangeAnimationStateMessage(long contIndex, int32 newState, int32 oldState) : ControllerMessage(kPlayerChangeAnimationStateMessage, contIndex)
{
	this->oldAnimationState = oldState;
	this->newAnimationState = newState;
}

PlayerChangeAnimationStateMessage::~PlayerChangeAnimationStateMessage()
{
}

void PlayerChangeAnimationStateMessage::Compress(Compressor &data) const
{
	ControllerMessage::Compress(data);
	data << newAnimationState;
	data << oldAnimationState;

}

bool PlayerChangeAnimationStateMessage::Decompress(Decompressor &data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> newAnimationState;
		data >> oldAnimationState;
		return true;
	}
	return false;
}





