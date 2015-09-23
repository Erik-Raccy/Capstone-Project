#include "RequestRenameMessage.h"

using namespace C4;


RequestRenameMessage::RequestRenameMessage() : Message(kMessageNameChangeRequestMessage)
{

}

RequestRenameMessage::RequestRenameMessage(const char * name) : Message(kMessageNameChangeRequestMessage)
{
	newName = name;
}

RequestRenameMessage::~RequestRenameMessage()
{
}

// Empty constructor used in child classes. Passes the MessageType parameter upwards.
RequestRenameMessage::RequestRenameMessage(MessageType type) : Message(type)
{

}

// Simple initializing constructor for use in child classes.
RequestRenameMessage::RequestRenameMessage(MessageType type, const char *name) : Message(type)
{
	newName = name;
}

// Our Compress method has to add our newName value to the Compressor data variable
// passed by the engine to our name change request message.
void RequestRenameMessage::Compress(Compressor &data) const
{
	data << newName;
}

/* Our Decompress method extracts the information from the Data variable and places it
in our newName variable. Officially, we also add validation logic into this method
to determine the validity of the contents of this message. This isn't really needed
in this case though, so we just return true and pretend we did validation.
*/
bool RequestRenameMessage::Decompress(Decompressor &data)
{
	data >> newName;
	return (true);
}

NameChangeMessage::NameChangeMessage() : RequestRenameMessage(kMessageNameChangeMessage)
{
}

NameChangeMessage::NameChangeMessage(const char *name, PlayerKey pk) : RequestRenameMessage(kMessageNameChangeMessage, name)
{
	playerKey = pk;
}

NameChangeMessage::~NameChangeMessage()
{
}

void NameChangeMessage::Compress(Compressor &data) const
{
	data << playerKey;
	RequestRenameMessage::Compress(data);
}

bool NameChangeMessage::Decompress(Decompressor &data)
{
	data >> playerKey;
	RequestRenameMessage::Decompress(data);
	return(true); //normally we'd validate the data
}