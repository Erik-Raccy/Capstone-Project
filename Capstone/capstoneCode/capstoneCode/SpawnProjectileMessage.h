#pragma once

#include "MyMessages.h"

namespace C4
{
	class SpawnProjectileMessage : public Message
	{
	public:

		//Constructors
		SpawnProjectileMessage();
		SpawnProjectileMessage(int32 contIndex, unsigned_int32 projType, Point3D p, Vector3D shootDir);
		~SpawnProjectileMessage();

		//Variables
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

		void SetDirection(Vector3D v)
		{
			direction = v;
		}

		int32 GetContIndex(void) const
		{
			return contIndex;
		}

		void SetContIndex(int32 ind)
		{
			contIndex = ind;
		}

		unsigned_int32 GetProjType(void) const
		{
			return projType;
		}

	private:
		//Variables
		int32 contIndex;
		Point3D position;
		Vector3D direction;
		unsigned_int32 projType;
	};

}
