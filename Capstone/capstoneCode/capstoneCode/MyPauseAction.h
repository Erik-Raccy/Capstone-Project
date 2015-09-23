#ifndef MYPAUSEACTION_H
#define MYPAUSEACTION_H

//Engine Headers
#include "C4Input.h"

//My Headers
#include "Game.h"
#include "MyMessages.h"

namespace C4{

	class MyPauseAction : public Action
	{
	public:
		//Constructors
		MyPauseAction(unsigned_int32 type);
		~MyPauseAction();

		//Methods
		void Begin(void);
		void End(void);

	};
}
#endif