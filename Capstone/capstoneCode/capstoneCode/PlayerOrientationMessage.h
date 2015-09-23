#pragma once

#include "MyMessages.h"

namespace C4
{
	class PlayerOrientationMessage : public ControllerMessage
	{
	private:
		//Variables
		float orientation;

	public:
		//Constructors
		PlayerOrientationMessage(long contIndex);
		PlayerOrientationMessage(long contIndex, float _orientation);
		~PlayerOrientationMessage();

		//Accesors
		float GetOrientation(void) const
		{
			return orientation;
		}

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};

}
