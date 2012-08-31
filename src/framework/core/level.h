#ifndef CORE_LEVEL_H
#define CORE_LEVEL_H

#include <geom/point.h>
#include <core/entity.h>
#include <core/environment.h>
#include <cstddef>

namespace Core {

class LevelLoader;
class Level;

//---------- Core::__entity
// "empty" return value for Core::Level::GetMainEntity()
class __entity : public virtual Core::Entity {
    __entity() : hb_(Geom::Point(0, 0), 0, 0) {}
public:
    virtual void Loop(const Core::Environment&) {}
    virtual const Geom::Region& HitBox() const { return hb_; }
    virtual bool IsHitBoxDamaging() const { return false; }
    virtual unsigned long Clssid() const { return 0xDEADB33F; }
    virtual void OnCollision(const Core::Entity&, const Geom::Point&) {}
    virtual Entity* ClonePtr() const { return new __entity(*this); }
private:
    Geom::Rectangle hb_;

    friend class Core::Level;
};

//---------- Core::Level
class Level {
protected:
    Level() {}
public:
    //========== Level::Level
    static Level* New()
    {
        Level* ret = new Level();
        ret->env_.lvl_ = ret;
        return ret;
    }
    //========== Level::GetEnvironment
    Environment& GetEnvironment() { return env_; }
    //========== Level::AllowEntityToLoop
    bool AllowEntityToLoop(Entity*) { return true; }
    //========== Level::GetMainEntity
    Entity& GetMainEntity() { static Entity* e = new __entity(); return *e; }
    //========== Level::End
    bool End() { return true; }
private:
    //========== Level:: private fields
    Environment env_;

    //========== Level:: friends
    friend class Core::LevelLoader;
};

//---------- Core::LevelLoader
class LevelLoader {
public:
    //========== LevelLoader::GetLevel
    Level& GetLevel(const int) { static Level* l = Level::New(); return *l; }
    //========== LevelLoader::GetMainEntity
    Entity& GetMainEntity() { static Entity* p; return *p; }
};

} // namespace

#endif
