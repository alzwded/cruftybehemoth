#include <core/resource.h>
#include <core/resourcemanager.h>

//========== Resource::Resource
Core::Resource::Resource(const unsigned long _rid, const std::string& _path, Core::ResourceManager* _rm)
    : path_(_path)
    , rm_(_rm)
    //, rid_(_rid)
    , rc_(0)
    {}

void Core::Resource::DecrRefCnt()
{
    if(rc_-- == 0) {
        rm_->AddToBlackList(this);
    }
}

//========== Resource::Sp::Get
void* Core::Resource::Sp::Get()
{
    if(!rc_) {
        data_ = dad_->Get();
        rc_ = new int(1);
    } else {
        return data_;
    }
}

//========== Resource::Sp::Drop
void Core::Resource::Sp::Drop()
{
    if(rc_ && *rc_ && (--*rc_ == 0)) {
        delete rc_;
        dad_->DecrRefCnt();
        data_ = NULL;
    }
}
