#include "LeaderBoardState.h"
#include "LeaderBoardState.h"
#include "GameState.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "AssetLoader.h"
#include "Keyboard.h"
#include "Game.h"
#include <fstream>
#include <sstream>

LeaderBoardState::LeaderBoardState()
{
}

LeaderBoardState::~LeaderBoardState()
{
}

void LeaderBoardState::OnActivate(System* system, StateArgumentMap& args)
{
    menus_.clear();
    menus_.push_back("        **  LeaderBoard **");
    menus_.push_back("**********************************");
    menus_.push_back("     PlayerName          |    Score");
    std::ifstream scoreFile("score.txt");
    std::string text;
    std::string word;
    unsigned int score = 0;

    while (getline(scoreFile, text))
    {
        std::stringstream ss(text);
          ss >> word >> score;
        scoreTable_[word] = score;
    }
    scoreFile.close();
    for (auto i : scoreTable_)
    {
        std::string scoreData(i.first +"   "+ std::to_string(i.second) );
        menus_.push_back(scoreData);
    }
  
}

void LeaderBoardState::OnUpdate(System* system)
{
    Keyboard* keyboard = system->GetKeyboard();
    if (keyboard->IsKeyPressed(VK_ESCAPE))
    {
        system->SetNextState("PauseMenuState");
    }
}

void LeaderBoardState::OnRender(System* system)
{
    system->GetGraphics()->ClearFrame(0.0f, 0.0f, 0.0f, 0.0f);

    int currentLineY = 5;
    for (StringList::iterator stringIt = menus_.begin(), end = menus_.end();
        stringIt != end;
        ++stringIt)
    {
        currentLineY += system->GetGraphics()->GetFontEngine()->DrawText(*stringIt, 5, currentLineY, 0x1fff5fff, FontEngine::FONT_TYPE_SMALL);
    }
}

void LeaderBoardState::OnDeactivate(System* system)
{
}

