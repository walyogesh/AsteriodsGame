#pragma once

#include "GameState.h"
#include <list>
#include <string>

class AssetLoader;
class Graphics;

class PauseMenuState : public GameState
{
public:
    PauseMenuState();
    ~PauseMenuState();

    void OnActivate(System* system, StateArgumentMap& args);
    void OnUpdate(System* system);
    void OnRender(System* system);
    void OnDeactivate(System* system);

private:
    typedef std::list<std::string> StringList;
    StringList Menus_;
};
