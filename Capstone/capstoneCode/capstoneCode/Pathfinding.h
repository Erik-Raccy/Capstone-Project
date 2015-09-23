#pragma once

#include "C4Engine.h"
#include "C4Node.h"
#include "C4World.h"
#include <list>
#include <vector>
#include "MyPlayerManager.h"

namespace C4
{
	class Point : Vector3D
	{
	public:
		Point();
		Point(Vector3D _vec)
		{
			this->x = _vec.x;
			this->y = _vec.y;
			this->z = _vec.z;
		}
		Vector3D getVector()
		{
			return Vector3D(x, y, z);
		}
		float f = 0;
		float g = 0;
		float h = 0;
	};
	
	//this class encapsulates the A* pathfinding for the game
	class Pathfinding
	{
	public:
		Pathfinding();
		~Pathfinding();

		std::vector<Vector3D*> aStar(Vector3D _start, Vector3D _end);
		float heuristic_cost_estimate(Point * _start, Point * _goal);

	private:

	};

}