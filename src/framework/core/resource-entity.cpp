#include <core/resource-entity.h>

//========== Resource_Entity::Load
void Core::Resource_Entity::Load()
{
    loaded_ = true;
}

//========== Resource_Entity::Unload
void Core::Resource_Entity::Unload()
{
    loaded_ = false;
}
