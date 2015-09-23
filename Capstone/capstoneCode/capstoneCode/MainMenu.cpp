#include "Game.h"
#include "C4Input.h"
#include "MainMenu.h"

using namespace C4;

MainWindow *C4::TheMainWindow = nullptr;

MainWindow::MainWindow() : Window("panel/Main"), Singleton<MainWindow>(TheMainWindow)
{
}

MainWindow::~MainWindow()
{
	if (mainBackground)
	{
		this->RemoveSubnode(mainBackground);
		delete mainBackground;
	}
}

void MainWindow::Preprocess()
{
	Window::Preprocess();
	//Widgets
	hostGame = static_cast<PushButtonWidget *>(this->FindWidget("hostGame"));
	joinGame = static_cast<PushButtonWidget *>(this->FindWidget("joinGame"));
	quitGame = static_cast<PushButtonWidget *>(this->FindWidget("quitGame"));
	ipAddress = static_cast<EditTextWidget *>(this->FindWidget("ipAddress"));
	mainBackground = static_cast<ImageWidget *> (this->FindWidget("MainBackground"));

	//Make the main menu in the middle of the screen
	mainBackground->SetWidgetSize(Vector2D(TheDisplayMgr->GetDisplayWidth(), TheDisplayMgr->GetDisplayHeight()));
	mainBackground->SetWidgetPosition(Point3D(-TheDisplayMgr->GetDisplayWidth()/2.0 +200, -TheDisplayMgr-> GetDisplayHeight()/2.0 +92,0));

	// Put the local IP in the title so we don't have to look it up
	ipAddress->SetText(MessageMgr::AddressToString(TheNetworkMgr->GetLocalAddress()));
}

//Handle when the player uses one of the widgets
void MainWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData)
{
	if (eventData->eventType == kEventWidgetActivate)
	{
		//If the player presses the Host button
		if (widget == hostGame)
		{
			TheGame->HostGame();
			this->Close();
		}
		//If the player presses the Join button
		else if (widget == joinGame)
		{
			TheGame->JoinGame(ipAddress->GetText());
			this->Close();
		}
		// If the player presses the quit button
		else if (widget == quitGame)
		{
			TheGame->QuitGame();
		}
	}
}
// Opens the main menu
void MainWindow::Open()
{
	if (TheMainWindow)
	{
		TheInterfaceMgr->SetActiveWindow(TheMainWindow);
	}
	else
	{
		TheInterfaceMgr->AddWidget(new MainWindow);
	}
}

void MainWindow::GraphicsSettingsComplete(GraphicsSettingsWindow *window, void *cookie)
{
	Open();
}