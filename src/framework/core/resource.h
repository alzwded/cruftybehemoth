#ifndef CORE_RESOURCE_H
#define CORE_RESOURCE_H

#include <string>
#include <vector>
#include <cstdarg>

namespace Core {

class ResourceManager;
class Resource;

//---------- ResourceManager:: typedefs
typedef Resource* (*Resource_cstr)(const unsigned long, const std::string&, Core::ResourceManager*);

//---------- Core::Resource
class Resource {
public:
    //========== Resource:: typedefs
    //---------- Resource::Sp
    class Sp {
    public:
        Sp(Resource* _dad)
            : dad_(_dad)
        {
            dad_->rc_++;
        }
        Sp(const Sp& _other) { Copy(_other); }
        Resource* Access()
        {
            if(!dad_->Loaded()) {
                //dad_->rc_->LoadResource(dad_->rc_); // TODO
                dad_->rc_->Get(dad_->rc_->GetRID(dad_));
            }
            return dad_;
        }
        bool operator!() { return !dad_; }
        Sp& operator=(const Sp& _other)
        {
            Drop();
            Copy(_other);
            return *this;
        }
        ~Sp() { Drop(); }
        /*template<typename T>
        T* GetAs() { return reinterpret_cast<T*>(Access()); }*/
    private:
        void Drop();
        void Copy(const Sp& _other);
    protected:
        Resource* dad_;

        friend class Core::ResourceManager;
        friend class Core::Resource;
    };

    //---------- Resource::Sp_of
    template<typename T>
    class SP_of
        : public Resource::SP
    {
        SP_of(Resource* _r)
            : SP(_r)
            {}
    public:
        SP_of<T> New(Resource* _r)
        {
            SP_of<T> ret(dynamic_cast<T*>(_r));
            return ret;
        }
        SP_of<T>& operator=(const Resource::SP& _other)
        {
            Drop();
            Copy(_other);
        }
        SP_of(const Resource::SP& _other)
        {
            if(!_other) return;
            Copy(_other);
        }
        virtual ~SP_of() {}
        T* operator->() { return static_cast<T*>(Access()); }

        friend class Core::ResourceManager;
        friend class Core::Resource;
    };
protected:
    //========== Resrouce::Resource
    Resource(const unsigned long _rid, const std::string& _path, ResourceManager* _rm);
    //========== Resrouce::~Resource
    virtual ~Resource() {}
    //========== Resrouce::IncrRefCnt
    void IncrRefCnt();
    //========== Resrouce::DecrRefCnt
    void DecrRefCnt();
public:
    //========== Resrouce::CLSSID
    static const unsigned long CLSSID = 0xD00078;
    //========== Resrouce::Clssid
    virtual unsigned long Clssid() const { return CLSSID; }
    //========== Resrouce::IsA
    virtual bool IsA(unsigned long _clssid) const { return CLSSID == _clssid; };

protected:
    //========== Resrouce::Load
    virtual void Load() =0;
    //========== Resrouce::Loaded
    virtual bool Loaded() =0;
    //========== Resrouce::Release
    virtual void Release() =0;

protected:
    //========== Resrouce::SetPath
    void SetPath(const std::string& _path) { path_ = _path; }
    //========== Resrouce::GetPath
    std::string GetPath() const { return path_; }
    //========== Resrouce::SetResourceManager
    void SetResourceManager(Core::ResourceManager* _rm) { rm_ = _rm; }
    //========== Resrouce::GetResourceManager
    Core::ResourceManager* GetResourceManager() const { return rm_; }

private:
    //========== Resrouce:: private fields
    std::string path_;
    ResourceManager* rm_;
    int rc_;

    //========== Resrouce:: friends
    friend class Core::ResourceManager;
    friend class Core::SP_Base;
};

} // namespace

#endif
