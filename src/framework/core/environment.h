#ifndef CORE_ENVIRONMENT_H
#define CORE_ENVIRONMENT_H

#include <list>
#include <utility>
#include <map>
#include <core/screenbuckettree.h>
#include <cstddef>

namespace Core {

//__________ uses
class Game;
class Level;
class Screen;
class Entity;
class Entity_Exit;

//---------- Core::Environment
class Environment {
public:
    //========== Environment::Environment
    Environment(const bool _useCache =true)
        : lvl_(NULL)
        , useCache_(_useCache)
        {}
    //========== Environment::PlaySounds
    void PlaySounds() {} // TODO
    //========== Environment::GetEntitiesNear
    // get entities within this 9-screen rectangle space
    EntityList GetEntitiesNear(const Entity&);
    //========== Environment::GetScreensFor
    // Entity should most of the time be Entity_PC
    ScreenList GetScreensFor(const Entity&);
    //========== Environment::GetScreenEntityIsOn
    const Screen* GetScreenEntityIsOn(const Entity&);
    //========== Environment::GetLevel()
    Level* GetLevel() { return lvl_; }
    //========== Environment::SetUseCache
    void SetUseCache(const bool _flag) { useCache_ = _flag; }
    //========== Environment::ClearCache
    void ClearCache();
private:
    //========== Environment::UpdateLocation
    void Move(Entity&);
    //========== Environment::GetScreensFor(Point)
    ScreenList GetScreensFor(const Geom::Point&);

private:
    //========== Environment:: private fields
    ScreenBucketTree screens_;
    std::list<std::pair<Entity_Exit*, Environment> > exits_;
    // if Entity_PC.HitBox().Intersects(Entity_Exit.HitBox())
    //      switch to that environment
    // Entity_Exit should be in the bucket tree

    //========== Environment:: cache
    //std::map<Geom::Point, EntityList> C_entities_;
    //std::map<Geom::Point, ScreenList> C_screens_;
    EntityList C_entities_;
    ScreenList C_screens_;
    Level* lvl_;
    bool useCache_;

    //========== Environment:: friends
    friend class Core::Game;
    friend class Core::Level;
};

} // namespace

#endif
