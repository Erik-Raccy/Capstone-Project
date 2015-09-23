#include "MyWorld.h"

using namespace C4;

//when constructer for my world is called, call parent world class constructor with same name
MyWorld::MyWorld(const char *name) : World(name)
{
}

MyWorld::~MyWorld()
{
}

ResourceResult MyWorld::Preprocess(void)
{
	WorldResult result = World::Preprocess(); //call the parent class preprocces
	return (kWorldOkay);
}

void MyWorld::Render(void)
{
	if (Init)
	{
		if (!loaded)
		{
			loaded = true;
		}
		if (loaded)
		{
		}
	}

	else
	{
		World *world = TheWorldMgr->GetWorld();
		if (world)
		{
			Init = !Init;
		}
	}
	World::Render();
}
