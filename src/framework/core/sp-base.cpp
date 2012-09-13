#include <core/sp-base.h>
#include <core/resource.h>

//========== SP_Base::SP_Base
Core::SP_Base::SP_Base(Core::Resource* _r)
{
    _r->IncrRefCnt();
}

//========== SP_Base::~SP_Base
Core::SP_Base::~SP_Base()
{
    _r->DecrRefCnt();
}
