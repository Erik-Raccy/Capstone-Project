#pragma once

#include "C4Engine.h"
#include "MyBaseState.h"

namespace C4
{

	class MyAttackState : public MyBaseState
	{
	public:
		MyAttackState(MyAIController * _cont);
		~MyAttackState();

		void StateEnter(unsigned_int32 _previous) override;
		void StateUpdate() override;
		void StateExit(unsigned_int32 _next) override;

	private:
		float attackTimer = 0;
	};

}