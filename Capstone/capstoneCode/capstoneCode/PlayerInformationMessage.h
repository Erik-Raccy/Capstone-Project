#pragma once

#include "MyMessages.h"

namespace C4
{
	class PlayerInformationMessage : public ControllerMessage
	{
	private:

		float messageHealth;
		float messageFuel;
		float messageScraps;

	public:

		//Constructors
		PlayerInformationMessage(long contIndex, float health, float fuel, float scraps);
		~PlayerInformationMessage();

		//Accesors
		float GetHealth() const
		{
			return messageHealth;
		}
		float GetFuel() const
		{
			return messageFuel;
		}

		float GetScraps() const
		{
			return messageScraps;
		}

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};

}
