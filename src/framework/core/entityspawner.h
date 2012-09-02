#ifndef CORE_ENTITYSPAWNER_H
#define CORE_ENTITYSPAWNER_H

#include <cstddef>
#include <core/entity.h>
#include <geom/point.h>

namespace Core {

class Resource_Entity;

//---------- Core::EntitySpawner
class EntitySpawner {
protected:
    //========== EntitySpawner::EntitySpawner
    EntitySpawner() {}
public:
    //========== EntitySpawner::~EntitySpawner
    virtual ~EntitySpawner() {}
    //========== EntitySpawner::Spawn(x,y)
    // returns a new instance of the loaded entity
    // returned by Resource_Entity::Get
    virtual Entity* Spawn(const Geom::Point&) =0;
    //========== EntitySpawner::Spawn
    Entity* Spawn(const float _x, const float _y)
    {
        return Spawn(Geom::Point(_x, _y));
    }
protected:
    //========== EntitySpawner::Set
    // set data to entity when spawning it
    // you should call this when overriding
    // TODO needs to be optimized...
    virtual void Set(Entity*);
private:
    //========== EntitySpawner:: private fields
    Geom::Point initialvelocity_;
    Core::BlockBundle blocks_;

    friend class Core::Resource_Entity;
};

} // namespace

#endif
