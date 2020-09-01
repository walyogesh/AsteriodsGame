#include "PlayingState.h"
#include "System.h"
#include "Game.h"
#include "FontEngine.h"
#include "Graphics.h"
#include "Keyboard.h"

PlayingState::PlayingState()
{
}

PlayingState::~PlayingState()
{
}

void PlayingState::OnActivate(System *system, StateArgumentMap &args)
{
    level_ = args["Level"].asInt;
}

void PlayingState::OnUpdate(System *system)
{
	Game *game = system->GetGame();
	game->Update(system);
	if (game->IsLevelComplete())
	{
		StateArgumentMap args;
		args["Level"].asInt  = level_ + 1; 
		game->AddPlayerLife(1); 
 		system->SetNextState("LevelStart", args);
	}
	else if (game->IsGameOver())
	{
		system->SetNextState("GameOver") ;
	}

    Keyboard* keyboard = system->GetKeyboard();
    if (keyboard->IsKeyPressed(VK_ESCAPE))
    {
        system->SetNextState("PauseMenuState");
    }
}

void PlayingState::OnRender(System *system)
{
	Game *game = system->GetGame();
	game->RenderEverything(system->GetGraphics());

	// draw score
	Graphics *graphics = system->GetGraphics();
	FontEngine *fontEngine = graphics->GetFontEngine();
	std::string score = std::to_string(system->GetGame()->GetScore());
	fontEngine->DrawText("Score: " + score, 700, 10, 0xffffffff, FontEngine::FONT_TYPE_SMALL);
}

void PlayingState::OnDeactivate(System *system)
{
}
