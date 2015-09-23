#pragma once

#include "MyMessages.h"

namespace C4
{
	class NewProjectileRequestMessage : public Message
	{
	public:
		//Constructors
		NewProjectileRequestMessage();
		NewProjectileRequestMessage(unsigned_int32 _projType, Point3D p, Vector3D shootDir);
		~NewProjectileRequestMessage();

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

		Vector3D GetDirection(void) const
		{
			return direction;
		}

		unsigned_int32 GetShotType() const
		{
			return projType;
		}

		void SetDirection(Vector3D v)
		{
			direction = v;
		}

	private:
		//Variables
		Point3D position;
		Vector3D direction;
		unsigned_int32 projType;
	};

}
