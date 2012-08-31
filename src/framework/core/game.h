#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <core/resourcemanager.h>
#include <core/level.h>
#include <core/inputadapter.h>
#include <time.h>
#include <string>
#include <cstddef>

#ifndef CORE_VERSION
#define CORE_VERSION "0.eternal-beta"
#endif

namespace Core {

//---------- Core::Game
class Game {
protected:
    //========== Game::_MainInit
    // initialize the display and input adapters (and possibly other stuff)
    virtual void _MainInit(int argc, char* argv[]) {}
    //========== Game::_MainLoop
    // run after rendering screens and running loop on entities
    virtual void _MainLoop(Level& currentLevel) {}
    //========== Game::_MainUninit
    // clean up
    virtual void _MainUninit() {}
    //========== Game::_ResourceManager
    ResourceManager& _ResourceManager() { return resources_; }

public:
    //========== Game::Game
    Game(
            DisplayAdapter* _display =NULL,
            InputAdapter* _input =NULL)
        : display_(_display)
        , input_(_input)
        , frameSpeed_(17)
        {}
    //========== Game::~Game
    virtual ~Game() {}
    //========== Game::Main
    int Main(int argc, char* argv[]);

private:
    //========== Game::MainLoop
    void MainLoop();
    //========== Game::GetHelp
    std::string GetHelp();
    //========== Game::Sleep
    void Sleep();

    //========== Game:: private fields
    ResourceManager resources_;
    LevelLoader levels_;
    DisplayAdapter* display_;
    InputAdapter* input_;
    clock_t frameBegin_;
    int frameSpeed_;
};

} // namespace

#endif
