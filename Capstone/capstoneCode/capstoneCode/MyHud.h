#ifndef MYHUD_H
#define MYHUD_H

/*
* File:  MyHud.h
* --------------------------
* This file contains the MyHud class,
* this class draws and updates the player's
* HUD. This involves the creation and modification
* of the UI widgets.
*/

//Engine Headers
#include "Game.h"
//My Headers

namespace C4
{
	class MyHud: public Board 
	{
	public:
		//constructors
		MyHud();
		~MyHud();
		
		//methods
		void Move();
		void Preprocess();	

	private:

		// Methods to update the GUI
		void UpdateHealth(float totalHealth, float currentHealth);
		void UpdateFuel(float totalFuel, float currentFuel);
		void UpdateAmountOfScraps(float amount);

		// The objects that get drawn on the screen
		QuadWidget *backgroundHealth;
		QuadWidget *backgroundFuel;
		QuadWidget *backgroundScraps;
		QuadWidget *healthBar;
		QuadWidget *fuelBar;
		TextWidget *healthText;
		TextWidget *fuelText;
		TextWidget *scrapsText;

		// Sizes of the screen
		float displayWidth = TheDisplayMgr->GetDisplayWidth();
		float displayHeight = TheDisplayMgr->GetDisplayHeight();
		float barSize;
		Vector2D windowSize = Vector2D(displayWidth, displayHeight);



	};

}

#endif