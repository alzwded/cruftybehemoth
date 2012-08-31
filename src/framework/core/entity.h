#ifndef CORE_ENTITY_H
#define CORE_ENTITY_H

#include <list>
#include <set>
#include <cstddef>
#include <geom/region.h>
#include <geom/point.h>
#include <core/blockbundle.h>
#include <core/util.h>
#include <debug/d123.h>

namespace Core {

//__________ uses
class Environment;
class DisplayAdapter;
class EntitySpawner;
class Game; // TODO maybe it should not be the one to call OnCollision

//---------- Geom::Entity
class Entity {
public:
    Entity(const Geom::Point& _location = Geom::Point())
        : id_(Core::ID::Next())
        , location_(_location)
        {}
    //========== Entity::~Entity
    virtual ~Entity() {}
    //========== Entity::Loop
    // AI loop
    // TODO break into Loop_Decision, (Resolve_Collisions), Loop_Perform
    virtual void Loop(const Environment&) =0;
    //========== Entity::HitBox
    virtual const Geom::Region& HitBox() const =0;
    //========== Entity::IsHitBoxDamaging
    virtual bool IsHitBoxDamaging() const =0;
    //========== Entity::Clssid
    virtual unsigned long Clssid() const =0;
    //========== Entity::IsA
    virtual bool IsA(const unsigned long _clssid) const
    {
        return Clssid() == _clssid;
    }

    //========== Entity::ID
    unsigned long ID() const
    {
        if(id_ == 0) {
            throw "uninitialized id!";
        }
        return id_;
    }
    //========== Entity::operator<
    bool operator<(const Entity& _entity) const
    {
        return ID() < _entity.ID();
    }

protected:
    //========== Entity::OnColision
    // stuff like reduce hit points or set a flag to run away next frame
    // etc
    virtual void OnCollision(const Entity&, const Geom::Point&) =0;

public:
    //========== Entity::Colides
    bool Collides(const Entity& _other)
    {
        D123_LOG(D123::FATAL, "TODO implement this to account for velocity");
        // TODO this will be moved in the main loop
        return HitBox().Intersects(_other.HitBox());
    }

    //========== Entity::GetLocation
    const Geom::Point& GetLocation() const
    {
        return location_;
    }
    //========== Entity::GetVelocity
    const Geom::Point& GetVelocity() const
    {
        return velocity_;
    }
    //========== Entity::GetBlockBundle
    const BlockBundle& GetBlockBundle() const
    {
        return blocks_;
    }
    //========== Entity::GetEntities
    std::list<Entity*>::const_iterator GetEntities() const
    {
        return entities_.begin();
    }
    //========== Entity::IsGhost
    bool IsGhost() const
    {
        return ghost_;
    }
    //========== Entity::ClonePtr
    virtual Entity* ClonePtr() const =0;

protected:
    //========== Entity::_GetVelocity
    Geom::Point& _GetVelocity()
    {
        return velocity_;
    }
    //========== Entity::_GetBlockBundle
    BlockBundle& _GetBlockBundle()
    {
        return blocks_;
    }
    //========== Entity::_GetEntities
    std::list<Entity*>& _GetEntities()
    {
        return entities_;
    }
    //========== Entity::SetGhost
    void SetGhost(const bool _flag)
    {
        ghost_ = _flag;
    }

private:
    //========== Entity::_GetLocation
    // private for inheriting classes
    // this will only be modified by the main loop
    Geom::Point& _GetLocation()
    {
        return location_;
    }
    //========== Entity:: private fields
    Geom::Point location_;
    Geom::Point velocity_;
    BlockBundle blocks_;
    std::list<Entity*> entities_;
    bool ghost_;
    unsigned long id_;

    //========== Entity:: friends
    friend class Core::EntitySpawner;
    friend class Core::Environment;
    friend class Core::DisplayAdapter;
    friend class Core::Game;
};

//__________ typedefs
typedef std::set<Entity*, DerefLess<Entity> > EntitySet;
typedef std::list<Entity*> EntityList;

} // namespace

#endif
