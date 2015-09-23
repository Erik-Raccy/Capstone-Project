#include "Pathfinding.h"


using namespace C4;

Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
	
}

std::vector<Vector3D*> Pathfinding::aStar(Vector3D _start, Vector3D _end)
{
	std::vector<Vector3D*> path;

	// f = g + h
	ThePlayerManager->PathNodeList;

	std::vector<Point *> closedList;   // The set of nodes already evaluated. (starts empty)
	std::vector<Point *> openList;// The set of tentative nodes to be evaluated, initially containing the start node
	Point * start = new Point(_start);
	Point * goal = new Point(_end);
	openList.push_back(start);
	std::vector<Point *> came_from;// The map of navigated nodes. starts empty

	start->g = 0; // Cost from start along best known path.
	start->f = start->g + heuristic_cost_estimate(start, goal); // Estimated total cost from start to goal through y.
	while (openList.empty() == false)
	{

	}
	return path;
}

float Pathfinding::heuristic_cost_estimate(Point * _start, Point * _goal)
{		
	return Magnitude(_start->getVector() - _goal->getVector());
}