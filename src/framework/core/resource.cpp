#include <core/resource.h>
#include <core/resourcemanager.h>

//========== Resource::Resource
Core::Resource::Resource(const unsigned long _rid, const std::string& _path, Core::ResourceManager* _rm)
    : path_(_path)
    , rm_(_rm)
    , rc_(0)
    {}

//========== Resource::Get
void Core::Resource::DecrRefCnt()
{
    if(rc_-- == 0) {
        rm_->AddToBlackList(this);
    }
}

//========== Resource::DecrRefCnt
void Core::Resource::IncrRefCnt()
{
    if(rc_ < 0) rc_ = 0;
    rc_++;
}

//========== Resource::DecrRefCnt
void Core::Resource::DecrRefCnt()
{
    if(rc_ && --rc_ == 0) {
        GetResourceManager()->AddToBlackList(this);
    }
}

//========== Resource::Sp::Copy
void Core::Resource::Sp::Copy(const Core::Resource::Sp& _other)
{
    dad_ = _other.dad_;
    dad_->IncrRefCnt();
}

//========== Resource::Sp::Drop
void Core::Resource::Sp::Drop()
{
    if(rc_ && *rc_ && (--*rc_ == 0)) {
        delete rc_;
        dad_->DecrRefCnt();
    }
}
