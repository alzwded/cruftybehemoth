#ifndef CORE_RESOURCE_H
#define CORE_RESOURCE_H

#include <string>

namespace Core {

class ResourceManager;

//---------- Core::Resource
class Resource {
public:
    //========== Resource:: typedefs
    class Sp {
    public:
        Sp(const int _rid, ResourceManager* _god)
            : rid_(_rid)
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
    protected:
        void* Get();
        template<class T>
        T* GetAs() { return static_cast<T*>(Get()); }
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
        void* data_;
        ResourceManager* god_;

        friend class Core::ResourceManager;
        friend class Core::Resource;
    };
protected:
    //========== Resrouce::Resource
    Resource(const unsigned long _rid, const std::string& _path, ResourceManager* _rm);
public:
    //========== Resrouce::~Resource
    virtual ~Resource() {}
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
    // TODO rename to _Get
    virtual void* Get() =0;
    //========== Resrouce::Loaded
    virtual bool Loaded() =0;
    //========== Resrouce::ManagedGet
    // TODO rename to Get
    Sp ManagedGet(...)
    {
        //return sp_; // no
        return Sp(rid_, rm);
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
    Sp sp_;

    //========== Resrouce:: friends
    friend class Core::ResourceManager;
};

//---------- ResourceManager:: typedefs
typedef Resource* (*Resource_cstr)(const unsigned long, const std::string&, Core::ResourceManager*);

} // namespace

#endif
