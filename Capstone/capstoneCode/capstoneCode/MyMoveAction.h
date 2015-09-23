#ifndef MYMOVEACTION_H
#define MYMOVEACTION_H

//Engine Headers
#include "C4Input.h"

//My Headers
#include "Game.h"


namespace C4
{
	class MovementAction : public Action
	{
	public:
		//constructors, 
		MovementAction(unsigned_int32 type);
		~MovementAction();

		//methods
		void Begin(void);
		void End(void);

	private:
		unsigned_int32	movementType;
	};
}

#endif