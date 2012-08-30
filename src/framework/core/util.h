#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <cstddef>

namespace Core {

//---------- Core::ID
// provides runtime ids
class ID {
public:
    //========== ID::Next
    static unsigned long Next();
    //========== ID::Reset
    static void Reset();
private:
    //========== ID::current
    static unsigned long current;
};

template<class T>
struct DerefLess {
    bool operator()(const T* lhs, const T* rhs)
    {
        return (*lhs) < (*rhs);
    }
};

} // namespace

#endif
