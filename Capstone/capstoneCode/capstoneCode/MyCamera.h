#ifndef MYCAMERA_H
#define MYCAMERA_H

/*
* File:  MyCamera.h
* --------------------
* This file contains the Camera class
* this is a "smart" camera used by the player
* it detects objects obstructing the player and 
* zooms in or out accordingly
*/



//Engine Headers
#include "C4Cameras.h"
#include "C4Engine.h"
#include "Game.h"

//My Headers

namespace C4
{
	class MyCamera :public FrustumCamera
	{
	public:

		//Constructors
		MyCamera();
		~MyCamera();

		//<ethods
		void Preprocess(void);
		void Move(void);

		//Accessor
		float getCameraZ()
		{
			return cameraZ;
		}

	private:
		float cameraZ;

	};

}

#endif