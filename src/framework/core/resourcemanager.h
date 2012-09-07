#ifndef CORE_RESOURCEMANAGER_H
#define CORE_RESOURCEMANAGER_H

#include <core/time.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <utility>
#include <assert.h>
#include <cstddef>

#define CORE_RESOURCEMANAGER_DEFAULTPATH "./"
#define CORE_RESOURCEMANAGER_PATHSEPARATOR "/"

#define CORE_TIME_FRAME_THRESHOLD 1 //ms

namespace Core {

//__________ uses
class Level;
class Entity;

class ResourceManager;

//---------- Core::LevelLoader
class LevelLoader {
public:
    //========== LevelLoader::LevelLoader
    LevelLoader(ResourceManager* _rm)
        : rm_(_rm)
        {}
    //========== LevelLoader::DropLevel
    void DropLevel(const int _i);
    //========== LevelLoader::GetLevel
    Level* GetLevel(const int _i);
    //========== LevelLoader::PushLevel
    void PushLevel(const std::string& _path)
    {
        levels_.push_back(std::pair<std::string, unsigned long>(_path, 0));
    }
    //========== LevelLoader::ResourceManager
    ResourceManager* _ResourceManager() { return rm_; }
private:
    std::vector<std::pair<std::string, unsigned long> > levels_;
    ResourceManager* rm_;
};

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
            ++*rc_;
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
    Sp ManagedGet()
    {
        return sp_;
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

//---------- Core::ResourceManager
class ResourceManager {
public:
    //========== ResourceManager::ResourceManager
    ResourceManager()
        : path_(CORE_RESOURCEMANAGER_DEFAULTPATH)
        , levels_(this)
        {}
    //========== ResourceManager::SetPath
    void SetPath(const std::string& _path) { path_ = _path; }
    //========== ResourceManager::GetPath
    std::string GetPath() const { return path_; }
    //========== ResourceManager::Load
    bool Load();
    //========== ResourceManager::Unload
    void Unload();
    //========== ResourceManager::GetRegistry
    static std::map<std::string, Resource_cstr>& GetRegistry();
    //========== ResourceManager::Register
    static void Register(const std::string&, Resource_cstr);
    //========== ResourceManager::GetRID
    unsigned long GetRID(const std::string& _path);
    //========== ResourceManager::Get
    void* Get(const unsigned long _resourceID, const unsigned long _type =0);
    //========== ResourceManager::Release
    void Release(const unsigned long _resourceID);
    //========== ResourceManager::LevelLoader
    LevelLoader& _LevelLoader() { return levels_; }
    //========== ResourceManager::Gc
    unsigned long Collect(const unsigned long _time, const bool _force =false);
private:
    //========== ResourceManager:: private members
    std::set<unsigned long> keys_;
    std::map<unsigned long, Resource*> resources_;
    std::map<std::string, unsigned long> pathIDs_;
    std::string path_;
    std::queue<unsigned long> blackList_;
    LevelLoader levels_;
};

} // namespace

#endif
