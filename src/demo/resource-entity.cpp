#include <demo/resource-entity.h>
#include <demo/entity-demo.h>

//========== Resource_Entity::__init__
Demo::Resource_Entity::Initializer Demo::Resource_Entity::__init__;

//========== Resource_Entity::New
Core::Resource* Demo::Resource_Entity::New(const std::string& _s, Core::ResourceManager* _rm)
{
    return new Demo::Resource_Entity(_s, _rm);
}

//========== Resource_Entity::_Load
void Demo::Resource_Entity::_Load(FILE* f, Core::EntitySpawner* _es)
{
    Demo::EntitySpawner* es = dynamic_cast<Demo::EntitySpawner*>(_es);
    if(!_es) {
        D123_LOG(D123::ERROR, "NULL _es");
        return;
    }
    fscanf(f, "%d", &es->accelerating_);
}

//========== EntitySpawner::Set
void Demo::EntitySpawner::Set(Core::Entity* _e)
{
    Core::EntitySpawner::Set(_e);

    Demo::Entity_Demo* ret = dynamic_cast<Demo::Entity_Demo*>(_e);
    assert(ret);

    ret->accelerating_ = accelerating_;
}

//========== EntitySpawner::Spawn
Core::Entity* Demo::EntitySpawner::Spawn(const Geom::Point& _p)
{
    Demo::Entity_Demo* ret = new Entity_Demo(_p);
    Set(ret);
    return ret;
}

//========== Resource_Entity::Resource_Entity
Demo::Resource_Entity::Resource_Entity(const std::string& _s, Core::ResourceManager* _rm)
    : ::Core::Resource_Entity(_s, _rm)
    {}
