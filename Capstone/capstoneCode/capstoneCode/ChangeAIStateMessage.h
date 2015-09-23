#pragma once

#include "MyMessages.h"

namespace C4
{
	class ChangeAIStateMessage : public ControllerMessage
	{
	public:

		ChangeAIStateMessage(long controllerIndex);
		ChangeAIStateMessage(long controllerIndex, int32 state);
		~ChangeAIStateMessage();

		void Compress(Compressor &data) const override;
		bool Decompress(Decompressor &data) override;

		int32 GetState(void) const
		{
			return state;
		}

	private:

		int32 state;
	};

}