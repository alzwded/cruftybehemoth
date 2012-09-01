#ifndef CORE_ENTITYSPAWNER_H
#define CORE_ENTITYSPAWNER_H

#include <cstddef>
#include <core/entity.h>

namespace Core {

//---------- Core::EntitySpawner
class EntitySpawner {
protected:
    //========== EntitySpawner::EntitySpawner
    EntitySpawner() {}
public:
    //========== EntitySpawner::Spawn
    // returns a new instance of the loaded entity
    // returned by Resource_Entity::Get
    virtual Entity* Spawn(const float x, const float y) =0;
};

} // namespace

#endif
