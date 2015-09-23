#ifndef _GAME_PLAYER_H
#define _GAME_PLAYER_H

/*
* File:  MyPlayerType.h
* --------------------
* This file contains the GamePlayer class
* this class inherits from the player class
* and adds additional information such as the
* player's controller information. This is stored
* in the message manager.
*/


#include "MyMessages.h"

namespace C4 {

	enum MyPlayerType
	{
		kMyPlayerController =0,
		kMyRespawnController
	};

	//class prototypes
	class CharacterController;
	class MyCamera;

	class GamePlayer : public Player
	{
	public:
		//constructors
		GamePlayer(PlayerKey key);
		~GamePlayer();

		//accessors
		CharacterController * GetController()
		{
			return cont;
		}
		void SetController(CharacterController * controller, MyPlayerType _type)
		{
			cont = controller;
			type = _type;
		}
		MyPlayerType getType()
		{
			return type;
		}
		MyCamera * GetMyCamera()
		{
			return camera;
		}
		void SetMyCamera(MyCamera * camera)
		{
			camera = camera;
		}

	private:
		//Variables
		CharacterController * cont;
		MyCamera * camera;
		MyPlayerType type;
	};
}

#endif