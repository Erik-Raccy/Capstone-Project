#pragma once
//#include "C4Map.h"
//#include "Game.h"
#include "C4Engine.h"

namespace C4
{
	class MyAIController;
		
	class MyBaseState 
	{
	public:
		
		MyAIController * cont;
		MyBaseState(MyAIController * _cont);
		~MyBaseState();
		
		virtual void StateEnter(unsigned_int32 _previous) = 0;
		virtual void StateUpdate() = 0;
		virtual void StateExit(unsigned_int32 _next) = 0;

	};

}