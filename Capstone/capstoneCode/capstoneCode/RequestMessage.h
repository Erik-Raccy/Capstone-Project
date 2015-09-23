#ifndef RequestMessage_h
#define RequestMessage_h

#include "MyMessages.h"

namespace C4
{
	class RequestMessage : public Message
	{
	public:
		//Constructors
		RequestMessage();
		~RequestMessage();

		//Methods
		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);
	};
}

#endif

