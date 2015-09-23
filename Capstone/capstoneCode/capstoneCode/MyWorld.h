#ifndef MYWORLD_H
#define MYWORLD_H


/*
* File:  MyWorld.h
* --------------------------
* This file contains the game's World
* subclass, this class was used for test 
* code early, but isn't used for much now.
*/


//Engine Headers
#include "C4Engine.h"
#include "C4World.h"
#include "C4Primitives.h"

#include "EntityManager.h"

namespace C4 {

	class MyCamera; 

	class MyWorld : public World
	{
	private:
		bool Init = false;
		bool loaded = false;
	public:

		//constructors
		MyWorld(const char *name);
		~MyWorld();

		//methods
		WorldResult Preprocess(void);
		void Render(void);
		void GeneratePickups(void);
		


	};

}
#endif