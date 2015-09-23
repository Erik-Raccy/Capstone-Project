#ifndef MYSELFDESTRUCTACTION_H
#define MYSELFDESTRUCTACTION_H

//Engine Headers
#include "C4Input.h"

//My Headers
#include "Game.h"


namespace C4
{
	class SelfDestructAction : public Action
	{
	public:
		//constructors, 
		SelfDestructAction(unsigned_int32 type);
		~SelfDestructAction();

		//methods
		void Begin(void);
		void End(void);

	private:
		unsigned_int32 destructType;
	};
}

#endif