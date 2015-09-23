#pragma once

#include "C4Engine.h"
#include "MyBaseState.h"

namespace C4
{
	
	class MyIdleState : public MyBaseState
	{
	public:
		MyIdleState(MyAIController * _cont);
		~MyIdleState();
		
		void StateEnter(unsigned_int32 _previous) override;
		void StateUpdate() override;
		void StateExit(unsigned_int32 _next) override;

	};

}