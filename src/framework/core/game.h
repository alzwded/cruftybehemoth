#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <core/resourcemanager.h>
#include <core/level.h>
#include <core/inputadapter.h>
#include <core/time.h>
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

public:
    //========== Game::SetResourcePath
    void SetResourcePath(const char*);
    //========== Game::SetFrameSpeed
    void SetFrameSpeed(const char*);
    //========== Game::SetDebugLevel
    void SetDebugLevel(const char*);
    //========== Game::ShowHelp
    void ShowHelp(const char*);
    //========== Game::SetLogFile
    void SetLogFile(const char*);
private:
    //========== Game::MainLoop
    void MainLoop();
    //========== Game::HandleParameters
    void HandleParameters(int argc, char* argv[]);
    //========== Game::GetHelp
    std::string GetHelp();
protected:
    //========== Game::UpdateLevel
    // maybe it was triggered the next level maybe?
    // disable everything from moving and fade out?
    // spawn coin enemies because time ran out in bomberman?
    // to do these things and more, do them here
    virtual void UpdateLevel(Core::Level*& _currentLevel);
public:
    //========== Game::Sleep
    void Sleep();
    //========== Game::LevelLoader
    //LevelLoader& _LevelLoader() { return _ResourceManager()._LevelLoader(); }

    //========== Game:: private fields
    ResourceManager resources_;
    //LevelLoader levels_;
    DisplayAdapter* display_;
    InputAdapter* input_;
    Core::Time::core_time_t frameBegin_;
    long frameSpeed_;
};

} // namespace

#endif
