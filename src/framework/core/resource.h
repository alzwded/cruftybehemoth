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
    class Sp {
    public:
        Sp(Resource* _dad, ResourceManager* _god)
            : dad_(_dad)
            , god_(_god)
            , rc_(NULL)
            , data_(NULL)
            {}
        Sp(const Sp& _other) { Copy(_other); }
        void* operator->() { return Get(); }
        void* operator*() { return Get(); }
        bool operator!() { return !rc_; }
        Sp& operator=(const Sp& _other)
        {
            Drop();
            Copy(_other);
            return *this;
        }
        ~Sp() { Drop(); }
        void* Get();
        template<typename T>
        T* GetAs() { return reinterpret_cast<T*>(Get()); }
    private:
        void Drop();
        void Copy(const Sp& _other)
        {
            rid_ = _other.rid_;
            rc_ = _other.rc_;
            data_ = _other.data_;
            god_ = _other.god_;
            if(rc_) ++*rc_;
        }
    private:
        int rid_;
        int* rc_;
        Resource* dad_;
        ResourceManager* god_;
        void* data_;

        friend class Core::ResourceManager;
        friend class Core::Resource;
    };
protected:
    //========== Resrouce::Resource
    Resource(const unsigned long _rid, const std::string& _path, ResourceManager* _rm);
    //========== Resrouce::~Resource
    virtual ~Resource() {}
    //========== Resrouce::DecrRefCnt
    void DecrRefCnt();
public:
    //========== Resrouce::CLSSID
    static const unsigned long CLSSID = 0xD00078;
    //========== Resrouce::Clssid
    virtual unsigned long Clssid() const { return CLSSID; }
    //========== Resrouce::IsA
    virtual bool IsA(unsigned long _clssid) const { return CLSSID == _clssid; };
    //========== Resrouce::Load
    virtual void Load() =0;
    //========== Resrouce::Release
    virtual void Release() =0;
    //========== Resrouce::Get
    // TODO rename to _Get, do not use this
    virtual void* Get() =0;
    //========== Resrouce::Loaded
    virtual bool Loaded() =0;
    //========== Resrouce::ManagedGet
    // TODO rename to Get, use this
    Sp ManagedGet()
    {
        rc_++;
        return Sp(this, rm_);
    }

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
};

} // namespace

#endif
