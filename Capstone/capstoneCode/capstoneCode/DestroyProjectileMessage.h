#pragma once

#include "MyMessages.h"

namespace C4
{
	class DestroyProjectileMessage : public ControllerMessage
	{
	public:
		//Constructors
		DestroyProjectileMessage(long controllerIndex);
		~DestroyProjectileMessage();

		//Methods
		void Compress(Compressor &data) const override;
		bool Decompress(Decompressor &data) override;
	};

}