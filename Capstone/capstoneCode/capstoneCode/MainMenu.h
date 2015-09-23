#pragma once

/*
* File:  MainWindow.h
* --------------------------
* This file contains the main window singleton
* class. The class is responsible for drawing 
* the opening game menu and its buttons.
*/


#include "C4Interface.h"

using namespace C4;

namespace C4
{
	class GraphicsSettingsWindow;

	class MainWindow : public Window, public Singleton<MainWindow>
	{
	public:
		MainWindow();
		~MainWindow();

		static void Open();
		void Preprocess();
		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData);
		static void GraphicsSettingsComplete(GraphicsSettingsWindow *window, void *cookie);

	private:

		PushButtonWidget	*hostGame;
		PushButtonWidget	*joinGame;
		PushButtonWidget	*quitGame;
		EditTextWidget		*ipAddress;
		ImageWidget			*mainBackground;
	};

	extern MainWindow *TheMainWindow;
}