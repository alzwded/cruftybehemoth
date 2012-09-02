#include <core/entityspawner.h>

//========== EntitySpawner::Set
void Core::EntitySpawner::Set(Core::Entity* ret)
{
    ret->_GetVelocity() = initialvelocity_;
    ret->_GetBlockBundle() = blocks_;
}
