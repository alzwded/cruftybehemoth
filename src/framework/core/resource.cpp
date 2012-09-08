#include <core/resource.h>
#include <core/resourcemanager.h>

//========== Resource::Resource
Core::Resource::Resource(const unsigned long _rid, const std::string& _path, Core::ResourceManager* _rm)
    : path_(_path)
    , rm_(_rm)
    , sp_(_rid, _rm)
    {}

//========== Resource::Sp::Get
void* Core::Resource::Sp::Get()
{
    if(!rc_) {
        data_ = god_->Get(rid_);
        rc_ = new int(1);
    } else {
        return data_;
    }
}

//========== Resource::Sp::Get
void Core::Resource::Sp::Drop()
{
    if(rc_ && (--*rc_ == 0)) {
        delete rc_;
        god_->Release(rid_);
        data_ = NULL;
    }
}
