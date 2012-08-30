#ifndef CORE_RESOURCE_ENTITY_H
#define CORE_RESOURCE_ENTITY_H

#include <core/resourcemanager.h>
#include <core/entityspawner.h>
#include <cstddef>

namespace Core {

//---------- Core::Resouce_Entity
class Resource_Entity 
    : public virtual Core::Resource
{
public:
    //========== Resrouce_Entity::Resrouce_Entity
    Resource_Entity() : loaded_(false) {}
    //========== Resrouce_Entity::CLSSID
    static const unsigned long CLSSID = 1;
    //========== Resrouce_Entity::Clssid
    virtual unsigned long Clssid() const { return Resource_Entity::CLSSID; }
    //========== Resrouce_Entity::Load
    virtual void Load();
    //========== Resrouce_Entity::Loaded
    virtual bool Loaded() { return loaded_; }
    //========== Resrouce_Entity::Unload
    virtual void Unload();
    //========== Resource_Entity::Get
    virtual void* Get()
    {
        return &es_;
    }

    //========== Resrouce_Entity::_GetEntitySpawner
    EntitySpawner& _GetEntitySpawner() { return es_; }
    
private:
    //========== Resrouce_Entity:: private fields
    EntitySpawner es_;
    bool loaded_;
};

} // namespace

#endif
