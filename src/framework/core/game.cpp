#include <core/game.h>
#include <core/displayadapter.h>
#include <debug/d123.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>

//========== Game::HandleParameters
//========== Game::GetHelp
//========== Game::ShowHelp
//========== Game::SetResourcePath
//========== Game::SetLogFile
//========== Game::SetDebugLevel
//========== Game::SetFrameSpeed
#include "game.ii"

//========== Game::Main
int Core::Game::Main(int argc, char* argv[])
{
    HandleParameters(argc, argv);
    resources_.Load();
    _MainInit(argc, argv);
    //do while(MainLoop());
    MainLoop();
    _MainUninit();
    resources_.Unload();

    return 0;
}

//========== Game::UpdateLevel
void Core::Game::UpdateLevel(Core::Level*& _currentLevel)
{
    if(!_currentLevel) return;
    if(_currentLevel->Next() && _currentLevel->End()) {
        int old = _currentLevel->GetNumber();
        _currentLevel = _currentLevel->Next();
        //_LevelLoader().DropLevel(old);
    } else if(!_currentLevel->Next() && _currentLevel->End()) {
        /* exit! */
        /*int old = _currentLevel->GetNumber();
        _currentLevel = _LevelLoader().GetLevel(0);
        _LevelLoader().DropLevel(old);*/
    }
}

//========== Game::MainLoop
inline void Core::Game::MainLoop()
{
    Core::EntityList::iterator i, j;
    Core::EntityList entities;
    Core::ScreenList screens;
    Core::Level* lvl = NULL;//_LevelLoader().GetLevel(0);

    do {
        Core::Time::gettime(frameBegin_);
        Core::Entity* cameraBearingEntity = lvl->GetMainEntity();
        screens = lvl->GetEnvironment().GetScreensFor(*cameraBearingEntity);
        entities = lvl->GetEnvironment().GetEntitiesNear(*cameraBearingEntity);
        // TODO check if parallel is feasable
        // entity decides where it wants to go
        for(i = entities.begin(); i != entities.end(); ++i) {
            if(lvl->AllowEntityToLoop(*i)) {
                (*i)->Loop(*lvl);
            }
        }
        // entity is told it collides with other stuff
        for(i = entities.begin(); i != entities.end(); ++i) {
            const Core::Screen* screen =
                lvl->GetEnvironment().GetScreenEntityIsOn(**i);
            if(!screen) {
                D123_LOG(D123::FATAL, "entity %ld is not on any screen", (*i)->ID());
            } else if(screen->HitWall(**i)) {
                D123_LOG(D123::DEBUG, "TODO entity %ld HitWall", (*i)->ID());
                //(*i)->OnCollision(screen, collisionPoint);
            }
            for(j = entities.begin(); j != entities.end(); ++j) {
                if(i != j && (*i)->Collides(**j)) {
                    Geom::Point collisionPoint; // TODO

                    (*i)->OnCollision(**j, collisionPoint);

                    D123_LOG(D123::TRACE, "%ld hit %ld", (*i)->ID(), (*j)->ID());
                }
            }
        }
        // update locations
        for(i = entities.begin(); i != entities.end(); ++i) {
            lvl->GetEnvironment().Move(**i);
        }
        lvl->GetEnvironment().ClearCache();

        lvl->GetEnvironment().PlaySounds();

        // test
        //_ResourceManager()._LevelLoader().DropLevel(lvl->GetNumber());

        _MainLoop(*lvl);

        if(display_) {
            display_->Render(screens, resources_);
        } else {}

        UpdateLevel(lvl);

        Sleep();
    } while(lvl && !lvl->End());
}

//========== Game::Sleep
inline void Core::Game::Sleep()
{
    Core::Time::core_time_t now;
    Core::Time::gettime(now);

    long remaining = Core::Time::remainingMS(frameBegin_, now, frameSpeed_);

    if(remaining > 0) {
        remaining = _ResourceManager().Collect(remaining);
        if(remaining > 0) {
            usleep(1000 * remaining);
        } else {
            D123_LOG(D123::WARNING, "overworked garbage collector by %dms", remaining);
        }
    } else {
        D123_LOG(D123::WARNING, "overworked main loop by %dms", remaining);
    }
}
