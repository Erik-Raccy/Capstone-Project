#pragma once

#include "C4Engine.h"
#include "MyBaseState.h"

namespace C4
{

	class MyChaseState : public MyBaseState
	{
	public:
		MyChaseState(MyAIController * _cont);
		~MyChaseState();

		void StateEnter(unsigned_int32 _previous) override;
		void StateUpdate() override;
		void StateExit(unsigned_int32 _next) override;

		private:
			Vector3D originalForward;
	};

}