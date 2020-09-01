#include "GameOver.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "Game.h"
#include "string"
#include <fstream>
#include <ctime>

GameOver::GameOver() :
	delay_(0)
{
}

void GameOver::OnActivate(System *system, StateArgumentMap &args)
{
	std::ofstream scoreFile;
	scoreFile.open("score.txt", std::fstream::out | std::fstream::app);
	scoreFile << system->GetGame()->GetSessionName() << std::time(0)<<" " << system->GetGame()->GetScore() << std::endl;
	scoreFile.close();
	delay_ = 120;
}

void GameOver::OnUpdate(System *system)
{
	if (--delay_ == 0)
	{
		system->SetNextState("MainMenu");
	}
}

void GameOver::OnRender(System *system)
{
	Graphics *graphics = system->GetGraphics();
	FontEngine *fontEngine = graphics->GetFontEngine();

	system->GetGame()->RenderBackgroundOnly(graphics);

	const char *gameOverText = "Game Over";
	int textWidth = fontEngine->CalculateTextWidth(gameOverText, FontEngine::FONT_TYPE_LARGE);
	int textX = (800 - textWidth) / 2;
	int textY = (600 - 48) / 2;
	fontEngine->DrawText(gameOverText, textX, textY, 0xff00ffff, FontEngine::FONT_TYPE_LARGE);
}

void GameOver::OnDeactivate(System *system)
{
}
