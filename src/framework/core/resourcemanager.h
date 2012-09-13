#ifndef CORE_RESOURCEMANAGER_H
#define CORE_RESOURCEMANAGER_H

#include <core/time.h>
#include <core/resource.h>
#include <core/sp.h>
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
class Level_ref;
class Entity;

class ResourceManager;

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
    //========== ResourceManager::AddToBlackList
    void AddToBlackList(Resource* _r)
    {
        //blackList_.push(_r);
    }
    //========== ResourceManager::Release
    void Release(const unsigned long _resourceID);
    //========== ResourceManager::GetIntroLevel
    Level_ref GetIntroLevel();
    //========== ResourceManager::Gc
    unsigned long Collect(const unsigned long _time, const bool _force =false);
private:
    //========== ResourceManager:: private members
    std::set<unsigned long> keys_;
    std::map<unsigned long, Resource*> resources_;
    std::map<std::string, unsigned long> pathIDs_;
    std::string path_;
    std::queue<unsigned long> blackList_;
};

} // namespace

#endif
