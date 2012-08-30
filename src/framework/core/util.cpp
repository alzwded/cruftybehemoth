#include <core/util.h>

//========== ID::current
unsigned long Core::ID::current = 1;

//========== ID::Next
unsigned long Core::ID::Next()
{
    return Core::ID::current++;
}

//========== ID::Reset
void Core::ID::Reset()
{
    Core::ID::current = 1;
}
