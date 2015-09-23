#pragma once

#include "MyMessages.h"

namespace C4
{
	class PlayerStateMessage : public ControllerMessage
	{
	public:

		//Constructors
		PlayerStateMessage(long controllerIndex);
		PlayerStateMessage(long controllerIndex, Point3D p);
		~PlayerStateMessage();

		//Methods
		void Compress(Compressor &data) const override;
		bool Decompress(Decompressor &data) override;

		//Accessors
		Point3D GetPosition(void) const
		{
			return position;
		}
		
		void SetPosition(Point3D p)
		{
			position = p;
		}

	private:
		Point3D position;
	};

}