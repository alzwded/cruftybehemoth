#ifndef CORE_ENTITYSPAWNER_H
#define CORE_ENTITYSPAWNER_H

#include <cstddef>
#include <core/entity.h>
#include <geom/point.h>

// TODO collapse this into resource-entity
// TODO actually, remove EntitySpawner and just collapse the spawn method
// into Resource_Entity

namespace Core {

class Resource_Entity;

//---------- Core::EntitySpawner
class EntitySpawner {
public:
    struct SpEntity {
        // TODO _dad will be Resource_Entity
        // ACTUALLY, dad should be the resource manager and on rc==0,
        // it should tell the resource manager "kill ma dad, kthxbai
        // TODO FIXME use the regular Resource::Sp, lol
        SpEntity(EntitySpawner* dad, Entity* e)
            : me_(e)
            , rc_(new int(1))
            , dad_(dad)
            {}
        Entity* operator->() { return me_; }
        Entity& operator*() { return *me_; }
        SpEntity& operator=(const SpEntity& other)
        {
            Drop();
            Assign(other);
        }
        SpEntity(const SpEntity& other)
        {
            Assign(other);
        }
        ~SpEntity() { Drop(); }
    private:
        void Drop()
        {
            if(rc_ && --*rc_ == 0) {
                delete me_;
                delete rc_;
                //dad_->Release(); // TODO this should be the resource
            }
        }
        void Assign(const SpEntity& other)
        {
            rc_ = other.rc_;
            if(rc_) *rc_++;
            me_ = other.me_;
            dad_ = other.dad_;
        }

        Entity* me_;
        int* rc_;
        EntitySpawner* dad_;
    };
protected:
    //========== EntitySpawner::EntitySpawner
    EntitySpawner() {}
public:
    //========== EntitySpawner::~EntitySpawner
    virtual ~EntitySpawner() {}
    //========== EntitySpawner::Spawn(x,y)
    // returns a new instance of the loaded entity
    // returned by Resource_Entity::Get
    // TODO make protected
    virtual Entity* Spawn(const Geom::Point&) =0;
    //========== EntitySpawner::Spawn
    // TODO remove
    Entity* Spawn(const float _x, const float _y)
    {
        return Spawn(Geom::Point(_x, _y));
    }
    //========== EntitySpawner::ManagedSpawn
    SpEntity ManagedSpawn(const Geom::Point& _p)
    {
        return SpEntity(this, Spawn(_p));
    }
    //========== EntitySpawner::ManagedSpawn
    SpEntity ManagedSpawn(const float _x, const float _y)
    {
        return SpEntity(this, Spawn(Geom::Point(_x, _y)));
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
