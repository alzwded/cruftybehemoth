#ifndef DEMO_GAME_H
#define DEMO_GAME_H

#include <core/game.h>
#include <debug/d123.h>
#include <cstddef>

namespace Demo {

//---------- Core::Game
class Game 
    : public Core::Game
{
protected:
    //========== Game::_MainInit
    // initialize the display and input adapters (and possibly other stuff)
    virtual void _MainInit(int argc, char* argv[])
    {
        void* res = NULL;
            //_ResourceManager().Get(_ResourceManager().GetRID("data/test.png"));
        D123_LOG(D123::DEBUG, "res@%p", res);
    }
};

} // namespace

#endif
