#pragma once
#include "GameState.h"
#include <list>
#include <string>

class LeaderBoardState : public GameState
{
public:
     LeaderBoardState();
    ~LeaderBoardState();
    void OnActivate(System* system, StateArgumentMap& args);
    void OnUpdate(System* system);
    void OnRender(System* system);
    void OnDeactivate(System* system);
private:
    typedef std::list<std::string> StringList;
    typedef std::map<std::string, unsigned int> ScoreTable;

    StringList menus_;
    ScoreTable scoreTable_;
};

