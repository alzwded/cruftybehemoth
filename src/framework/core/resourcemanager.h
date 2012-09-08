#ifndef CORE_RESOURCEMANAGER_H
#define CORE_RESOURCEMANAGER_H

#include <core/time.h>
#include <core/resource.h>
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
