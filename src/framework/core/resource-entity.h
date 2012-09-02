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
    : public Core::Resource
{
protected:
    //========== Resrouce_Entity::Resrouce_Entity
    Resource_Entity(
            const std::string& _path,
            Core::ResourceManager* _rm);
public:
    //========== Resrouce_Entity::CLSSID
    static const unsigned long CLSSID = 1;
    //========== Resrouce_Entity::Clssid
    virtual unsigned long Clssid() const { return Resource_Entity::CLSSID; }
    //========== Resrouce_Entity::IsA
    virtual bool IsA(const unsigned long _clssid) const
    {
        return CLSSID == _clssid || Core::Resource::IsA(_clssid);
    }
private:
    //========== Resrouce_Entity::Load
    virtual void Load();
public:
    //========== Resource_Entity::_Load
    virtual void _Load(FILE* f, EntitySpawner*) =0;
    //========== Resrouce_Entity::Loaded
    // TODO move to abstract class
    virtual bool Loaded() { return es_ != NULL; }
    //========== Resrouce_Entity::Unload
    // TODO move to abstract class
    virtual void Release();
    //========== Resource_Entity::Get
    // TODO move to abstract class
    // no, it should not return Spawn, it should return the EntitySpawner
    virtual void* Get()
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
