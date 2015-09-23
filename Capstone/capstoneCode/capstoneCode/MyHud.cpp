#include "MyHud.h"

using namespace C4;

//Constructor that creates the Hud
MyHud::MyHud() : Board(windowSize)
{
	Preprocess();
	// Have the health bar go a quarter of the way across the window
	barSize = displayWidth / 4.0F;

	/*  --- Health Bar --- */

	//Black background of health bar
	backgroundHealth = new QuadWidget(Vector2D(barSize, 20.0F), ColorRGBA(0.0F, 0.0F, 0.0F, 0.5F));
	backgroundHealth->SetWidgetPosition(Point3D(10, 45.0F, 0.0F));
	AddSubnode(backgroundHealth);

	//Red part of health bar
	healthBar = new QuadWidget(Vector2D(barSize - 4, 16.0F), ColorRGBA(1.0F, 0.0F, 0.0F, 0.75F));
	healthBar->SetWidgetPosition(Point3D(12, 47.0F, 0.0F));
	AddSubnode(healthBar);

	// The text that says "Health:"
	healthText = new TextWidget(Vector2D(80, 16.0F), "Health:");
	healthText->SetFont("font/Gui");
	healthText->SetTextScale(1.25f);
	healthText->SetWidgetColor(ColorRGBA(1.0F, 0.0F, 0.0F, 1.0F));
	healthText->SetWidgetPosition(Point3D(10, 25.0F, 0.0F));
	healthText->SetTextAlignment(kTextAlignLeft);
	AddSubnode(healthText);

	/*  --- Fuel Bar --- */

	//Black background of fuel bar
	backgroundFuel = new QuadWidget(Vector2D(barSize, 20.0F), ColorRGBA(0.0F, 0.0F, 0.0F, 0.5F));
	backgroundFuel->SetWidgetPosition(Point3D(10, 90.0F, 0.0F));
	AddSubnode(backgroundFuel);

	//Green part of Fuel bar
	fuelBar = new QuadWidget(Vector2D(barSize - 4, 16.0F), ColorRGBA(0.0F, 1.0F, 0.0F, 0.75F));
	fuelBar->SetWidgetPosition(Point3D(12, 92.0F, 0.0F));
	AddSubnode(fuelBar);

	// The text that says "Fuel:"
	fuelText = new TextWidget(Vector2D(80, 16.0F), "Fuel:");
	fuelText->SetFont("font/Gui");
	fuelText->SetTextScale(1.25f);
	fuelText->SetWidgetColor(ColorRGBA(0.0F, 1.0F, 0.0F, 1.0F));
	fuelText->SetWidgetPosition(Point3D(10, 70.0F, 0.0F));
	fuelText->SetTextAlignment(kTextAlignLeft);
	AddSubnode(fuelText);

	/*  --- Scraps --- */

	//Black background of scraps
	backgroundScraps = new QuadWidget(Vector2D(200,35), ColorRGBA(0.0F, 0.0F, 0.0F, 0.5F));
	backgroundScraps->SetWidgetPosition(Point3D(displayWidth - barSize/2.0F, displayHeight - 80.0F, 0.0F));
	AddSubnode(backgroundScraps);

	// The text that says "Scraps: "
	scrapsText = new TextWidget(Vector2D(80, 16.0F), "Scraps: 0");
	scrapsText->SetFont("font/Bold");
	scrapsText->SetTextScale(1.25f);
	scrapsText->SetWidgetColor(ColorRGBA(1.0F, 1.0F, 1.0F, 1.0F));
	scrapsText->SetWidgetPosition(Point3D(displayWidth - barSize/2.0F + 10, displayHeight - 70.0F, 0.0F));
	scrapsText->SetTextAlignment(kTextAlignLeft);
	AddSubnode(scrapsText);
}

MyHud::~MyHud()
{
	this->RemoveSubnode(backgroundHealth);
	delete backgroundHealth;
	this->RemoveSubnode(backgroundFuel);
	delete backgroundFuel;
	this->RemoveSubnode(backgroundScraps);
	delete backgroundScraps;
	this->RemoveSubnode(healthBar);
	delete healthBar;
	this->RemoveSubnode(fuelBar);
	delete fuelBar;
	this->RemoveSubnode(healthText);
	delete healthText;
	this->RemoveSubnode(fuelText);
	delete fuelText;
	this->RemoveSubnode(scrapsText);
	delete scrapsText;
}

void MyHud::Preprocess(void)
{
	Board::Preprocess();
}

// Updates the GUI based on the values from the player
void MyHud::Move()
{
	GamePlayer *player = static_cast<GamePlayer*>(TheMessageMgr->GetLocalPlayer());
	if (player->getType() ==kMyPlayerController)
	{
		MyPlayerController* controller = static_cast<MyPlayerController *>(player->GetController());
		if (controller)
		{
			//Get information from the player
			float currentHealth = controller->getHealth();
			float currentFuel = controller->getFuel();
			float currentScraps = controller->getScraps();

			float totalHealth = controller->getTotalHealth();
			float totalFuel = controller->getMaxFuel();

			//Update the GUI
			UpdateHealth(totalHealth, currentHealth);
			UpdateFuel(totalFuel, currentFuel);
			UpdateAmountOfScraps(currentScraps);
		}
	}
}

// Updates the length of the health bar
void MyHud::UpdateHealth(float totalHealth, float currentHealth)
{
	float healthBarSize = (currentHealth*(barSize)) / totalHealth;
	healthBar->SetWidgetSize(Vector2D(healthBarSize, 16.0F));
}


// Updates the length of the fuel bar
void MyHud::UpdateFuel(float totalFuel, float currentFuel)
{
	float fuelBarSize = (currentFuel*(barSize)) / totalFuel;
	fuelBar->SetWidgetSize(Vector2D(fuelBarSize, 16.0F));
}

//Updates the amound of scrap
void MyHud::UpdateAmountOfScraps(float amount)
{
	scrapsText->SetText(Text::FloatToString(amount));
}