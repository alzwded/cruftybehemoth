#include <core/game.h>
#include <core/displayadapter.h>
#include <debug/d123.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>

//========== Game::GetHelp
inline std::string Core::Game::GetHelp()
{
    std::stringstream s;
    s << "game-framework-1 v" << CORE_VERSION << std::endl;
    s << "flags:" << std::endl;
    s << "\t--resource-path path/to/stuff   set the path to your resource dir" << std::endl;
    s << "\t--help                          print this message" << std::endl;
    s << "\t--frame-speed N                 set the speed of a frame (default 17ms" << std::endl;
    s << "\t--debug-level N                 set verbosity level" << std::endl;
    s << "\t--log-file path/to/file         set verbosity level" << std::endl;
    s << std::endl;
    return s.str();
}

//========== Game::Main
int Core::Game::Main(int argc, char* argv[])
{
    char* logFile = NULL;

    for(int i = 1; i < argc; ++i) {
        if(strncmp(argv[i], "--resource-path", 15) == 0) {
            if(i < argc) ++i;
            else {
                std::cerr << "invalid arguments" << std::endl;
                return -2;
            }
            resources_.SetPath(argv[i]);
        } else if(strncmp(argv[i], "--help", 6) == 0) {
            std::cout << GetHelp();
            return -1;
        } else if(strncmp(argv[i], "--frame-speed", 13) == 0) {
            if(i < argc) ++i;
            else {
                std::cerr << "invalid arguments" << std::endl;
                return -2;
            }
            frameSpeed_ = atoi(argv[i]);
        } else if(strncmp(argv[i], "--debug-level", 13) == 0) {
            if(i < argc) ++i;
            else {
                std::cerr << "invalid arguments" << std::endl;
                return -2;
            }
            D123::Debug::Level() = D123::Debug::LevelFromInt(atoi(argv[i]));
        } else if(strncmp(argv[i], "--log-file", 11) == 0) {
            if(i < argc) ++i;
            else {
                std::cerr << "invalid arguments" << std::endl;
                return -2;
            }
            logFile = argv[i];
        }
    }
    if(logFile)
    {
        FILE* f = fopen(logFile, "w");
        if(f) {
            D123::Debug::File() = f;
        } else {
            std::cerr << "cannot open " << logFile << std::endl;
        }
    }
    resources_.Load();
    _MainInit(argc, argv);
    //do while(MainLoop());
    MainLoop();
    _MainUninit();
    resources_.Unload();

    return 0;
}

//========== Game::MainLoop
void Core::Game::MainLoop()
{
    Core::EntityList::iterator i;
    Core::EntityList entities;
    Core::ScreenList screens;
    Core::Level lvl = levels_.GetLevel(0);

    do {
        frameBegin_ = clock();
        Core::Entity& cameraBearingEntity = lvl.GetMainEntity();
        screens = lvl.GetEnvironment().GetScreensFor(cameraBearingEntity);
        if(display_) {
            display_->Render(screens, resources_);
        } else {
        }
        entities = lvl.GetEnvironment().GetEntitiesNear(cameraBearingEntity);
        for(i = entities.begin(); i != entities.end(); ++i) {
            if(lvl.AllowEntityToLoop(*i)) {
                (*i)->Loop(lvl.GetEnvironment());
            }
        }
        lvl.GetEnvironment().ClearCache();

        lvl.GetEnvironment().PlaySounds();

        _MainLoop(lvl);

        Sleep();
    } while(!lvl.End());
}

//========== Game::Sleep
inline void Core::Game::Sleep()
{
    clock_t now = clock();
    if(now == frameBegin_) {
        usleep(1000 * frameSpeed_);
    } else if(now > frameBegin_) {
        usleep(1000 * frameSpeed_ - (now - frameBegin_));
    } else {
        usleep(1000 * frameSpeed_ - ((~now) + frameBegin_));
    }
}
