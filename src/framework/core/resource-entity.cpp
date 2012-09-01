#include <core/resource-entity.h>
#include <debug/d123.h>
#include <cstdio>

//========== Resource_Entity::Load
void Core::Resource_Entity::Load()
{
    FILE* f = fopen(GetPath().c_str(), "r");
    if(!f) {
        D123_LOG(D123::ERROR, "Could not load resource from %s", GetPath().c_str());
    }
    es_ = NewEntitySpawner();
    // read initial velocity, graphics blocks, (sound resources TODO)

    // call private _Load follows
    _Load(f);
    fclose(f);
}

//========== Resource_Entity::Unload
void Core::Resource_Entity::Unload()
{
    delete es_;
}
