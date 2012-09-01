#ifndef CORE_RESOURCE_ENTITY_H
#define CORE_RESOURCE_ENTITY_H

#include <core/resourcemanager.h>
#include <core/entityspawner.h>
#include <cstddef>

// abstract class, does basic reading (initial velocity, graphics blocks)
// have a virtual protected method that reads the rest
// this effectively becomes the new EntitySpawner

namespace Core {

//---------- Core::Resouce_Entity
class Resource_Entity 
    : public virtual Core::Resource
{
public:
    //========== Resrouce_Entity::Resrouce_Entity
    Resource_Entity() : es_(NULL) {}
    //========== Resrouce_Entity::CLSSID
    static const unsigned long CLSSID = 1;
    //========== Resrouce_Entity::Clssid
    virtual unsigned long Clssid() const { return Resource_Entity::CLSSID; }
private:
    //========== Resrouce_Entity::Load
    virtual void Load();
public:
    //========== Resource_Entity::_Load
    virtual void _Load(FILE* f) =0;
    //========== Resrouce_Entity::Loaded
    // TODO move to abstract class
    virtual bool Loaded() { return es_ != NULL; }
    //========== Resrouce_Entity::Unload
    // TODO move to abstract class
    virtual void Unload();
    //========== Resource_Entity::Get
    // TODO move to abstract class
    // no, it should not return Spawn, it should return the EntitySpawner
    virtual void* Get()
    {
        return es_;
    }
    //========== Resource_Entity::GetEntitySpawner
    EntitySpawner* GetEntitySpawner()
    {
        return es_;
    }

protected:
    //========== Resource_Entity::NewEntitySpawner
    // create a specific entity spawner
    virtual EntitySpawner* NewEntitySpawner() =0;

private:
    //========== Resrouce_Entity:: private fields
    EntitySpawner* es_;
};

} // namespace

#endif
