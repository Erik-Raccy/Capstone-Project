#include "RequestMessage.h"

using namespace C4;


RequestMessage::RequestMessage() : Message(kMessageRequest)
{
}

RequestMessage::~RequestMessage()
{
}

void RequestMessage::Compress(Compressor &data) const
{
}

bool RequestMessage::Decompress(Decompressor &data)
{
	return true;
}
