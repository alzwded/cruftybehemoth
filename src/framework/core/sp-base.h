#ifndef CORE_SP_BASE_H
#define CORE_SP_BASE_H

namespace Core {

class Resource;

class SP_Base {
public:
    SP_Base(Resource*);
    virtual ~SP_Base();
};

} // namespace

#endif
