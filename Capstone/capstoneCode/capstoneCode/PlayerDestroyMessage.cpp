#include "PlayerDestroyMessage.h"

using namespace C4;

PlayerDestroyMessage::PlayerDestroyMessage(long contIndex) : ControllerMessage(kPlayerMessageDestroy, contIndex)
{

}

PlayerDestroyMessage::~PlayerDestroyMessage()
{
}
