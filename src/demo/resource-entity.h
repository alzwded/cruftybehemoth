#ifndef DEMO_RESOURCE_ENTITY_H
#define DEMO_RESOURCE_ENTITY_H

#include <core/resourcemanager.h>
#include <core/resource-entity.h>
#include <core/entity.h>
#include <core/entityspawner.h>
#include <demo/entity-demo.h>
#include <string>

namespace Demo {

class Resource_Entity;

//---------- Demo::Entity_Spawner
class EntitySpawner
    : public virtual Core::EntitySpawner
{
public:
    //========== Entity_Spawner::Entity_Spawner
    EntitySpawner() : Core::EntitySpawner(), accelerating_(0) {}
    //========== Entity_Spawner::~Entity_Spawner
    virtual ~EntitySpawner() {}
    //========== Entity_Spawner::Spawn
    virtual Core::Entity* Spawn(const Geom::Point& _p);
protected:
    //========== Entity_Spawner::Set
    virtual void Set(Core::Entity*);
protected:
    //========== Entity_Spawner:: private fields
    int accelerating_;

    //========== Entity_Spawner:: friends
    friend class Demo::Resource_Entity;
};

//---------- Demo::Resource_Entity
class Resource_Entity
    : public Core::Resource_Entity
{
public:
    //========== Resource_Entity::Resource_Entity
    Resource_Entity(
            const unsigned long _rid,
            const std::string& _s, 
            Core::ResourceManager* _rm);
    //========== Resource_Entity::CLSSID
    static const unsigned long CLSSID = 0x80000101;
    //========== Resource_Entity::Clssid
    virtual unsigned long Clssid() const { return Demo::Resource_Entity::CLSSID; }
    //========== Resource_Entity::IsA
    virtual bool IsA(unsigned long _clssid) const
    {
        return Core::Resource_Entity::IsA(_clssid) || _clssid == Core::Resource_Entity::CLSSID;
    }
    //========== Resource_Entity::NewEntitySpawner
    virtual Core::EntitySpawner* NewEntitySpawner()
    {
        return new Demo::EntitySpawner();
    }
    //========== Resource_Entity::_Load
    virtual void _Load(FILE*, Core::EntitySpawner*);
    //========== Resource_Entity::New
    static Core::Resource* New(const unsigned long _rid, const std::string& _s, Core::ResourceManager* _rm);
private:
    //========== Resource_Entity:: static initializer
    struct Initializer {
        Initializer() {
            Core::ResourceManager::Register("demo-entity", &Resource_Entity::New);
        }
    };
    static Initializer __init__;
};

} // namespace

#endif
