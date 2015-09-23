#pragma once

#include "MyMessages.h"

namespace C4
{
	class PlayerChangeAnimationStateMessage : public ControllerMessage
	{
	public:

		PlayerChangeAnimationStateMessage(long controllerIndex);
		PlayerChangeAnimationStateMessage(long controllerIndex, int32 newState, int32 oldState);
		~PlayerChangeAnimationStateMessage();

		void Compress(Compressor &data) const override;
		bool Decompress(Decompressor &data) override;

		int32 GetNewAnimationState(void) const
		{
			return newAnimationState;
		}
		int32 GetOldAnimationState(void) const
		{
			return oldAnimationState;
		}
		//bool HandleMessage(Player *sender) const;

	private:

		int32 newAnimationState;
		int32 oldAnimationState;
	};

}