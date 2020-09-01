#include "PauseMenuState.h"
#include "GameState.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "AssetLoader.h"
#include "Keyboard.h"

PauseMenuState::PauseMenuState() 
{
}

PauseMenuState::~PauseMenuState()
{
}

void PauseMenuState::OnActivate(System* system, StateArgumentMap& args)
{
    Menus_.clear();
    Menus_.push_back("**  Menu **");
    Menus_.push_back("1. LeaderBoard:");
    Menus_.push_back("2. Exit:");
    Menus_.push_back("3. About:");
}

void PauseMenuState::OnUpdate(System* system)
{
    Keyboard* keyboard = system->GetKeyboard();
    if (keyboard->IsKeyPressed(VK_ESCAPE))
    {
        system->SetNextState("PlayingState");
    }
    if (keyboard->IsKeyPressed(VK_NUMPAD1))
    {
        system->SetNextState("LeaderBoardState");
    }
    if (keyboard->IsKeyPressed(VK_NUMPAD2))
    {
        system->SetNextState("PlayingState");
    }
    if (keyboard->IsKeyPressed(VK_NUMPAD3))
    {
        Menus_.clear();
        Menus_.push_back("**  Menu **");
        Menus_.push_back("**  About ME **");
    }
}

void PauseMenuState::OnRender(System* system)
{
    system->GetGraphics()->ClearFrame(0.0f, 0.0f, 0.0f, 0.0f);

    int currentLineY = 5;
    for (StringList::iterator stringIt = Menus_.begin(), end = Menus_.end();
        stringIt != end;
        ++stringIt)
    {
        currentLineY += system->GetGraphics()->GetFontEngine()->DrawText(*stringIt, 5, currentLineY, 0xffffffff, FontEngine::FONT_TYPE_SMALL);
    }
}

void PauseMenuState::OnDeactivate(System* system)
{
}
