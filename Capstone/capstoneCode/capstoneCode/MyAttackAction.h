#ifndef MYATTACKACTION_H
#define MYATTACKACTION_H

//Engine Headers
#include "C4Input.h"

//My Headers
#include "Game.h"
#include "MyMessages.h"

namespace C4{

	class MyAttackAction : public Action
	{
	public:
		//Constructors
		MyAttackAction(unsigned_int32 type);
		~MyAttackAction();

		//Methods
		void Begin(void);
		void End(void);

	private:
		unsigned_int32 attackType;
	};
}
#endif