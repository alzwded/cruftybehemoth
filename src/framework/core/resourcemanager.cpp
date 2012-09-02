#include <core/resourcemanager.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <debug/d123.h>

//========== ResourceManager::Load
bool Core::ResourceManager::Load()
{
    D123_LOG(D123::TRACE, "loading resources");
    FILE* f = fopen((GetPath() + "resources").c_str(), "r");

    if(!f) {
        D123_LOG(D123::ERROR, "could not load resource file!");
        return false;
    }

    char *pTag = NULL;
    char *pPath = NULL;
    std::map<std::string, Resource_cstr>::iterator i;
    while(!feof(f)) {
        pTag = NULL;
        pPath = NULL;
        assert(fscanf(f, "%a[^:]:%a[^\n]", &pTag, &pPath) == 2);
        if(!feof(f)) fscanf(f, "%*[\n]");

        assert(pTag);
        assert(pPath);
        std::string tag(pTag);
        std::string path(pPath);

        D123_LOG(D123::INFO, "adding resource |%s|%s|", tag.c_str(), path.c_str());
        assert(!tag.empty());
        assert(!path.empty());

        if((i = GetRegistry().find(tag)) != GetRegistry().end()) {
            unsigned long newID = 1;
            if(!keys_.empty()) newID = *(keys_.rbegin()) + 1;

            D123_LOGLINE(D123::INFO, "RID = %ld", newID);

            Resource* res = i->second(path, this);
            assert(res);


            keys_.insert(newID);
            resources_.insert(
                std::pair<unsigned long, Resource*>(
                    newID,
                    res));
            pathIDs_.insert(
                std::pair<std::string, unsigned long>(
                    path,
                    newID));
        } else {
            D123_LOG(D123::ERROR, "%s is not a registered resource tag", tag.c_str());
        }

        free(pTag);
        free(pPath);
    }

    fclose(f);

    return true;
}

//========== ResourceManager::Unload
void Core::ResourceManager::Unload()
{
    D123_LOG(D123::TRACE, "unloading resources");
    for(std::map<unsigned long, Resource*>::iterator i = resources_.begin();
        i != resources_.end();
        ++i)
    {
        D123_LOG(D123::TRACE, "deleting resource %ld", i->first);
        assert(i->second);
        i->second->Release(); // TODO should check if loaded? neah
        delete i->second;
    }
    keys_.clear();
    resources_.clear();
    pathIDs_.clear();
}

//========== ResourceManager::GetRegistry
std::map<std::string, Core::Resource_cstr>&
Core::ResourceManager::GetRegistry()
{
    static std::map<std::string, Resource_cstr> registry;
    return registry;
}

//========== ResourceManager::Register
void Core::ResourceManager::Register(
        const std::string& _tag,
        Core::Resource_cstr _resource)
{
    assert(_resource);
    assert(!_tag.empty());
    D123_LOG(D123::INFO, "registering %s", _tag.c_str());
    GetRegistry().insert(
        std::pair<std::string, Resource_cstr>(
            _tag,
            _resource));
}

//========== ResourceManager::GetRID
unsigned long Core::ResourceManager::GetRID(const std::string& _path)
{
    D123_LOG(D123::TRACE, "resource %s's RID requested", _path.c_str());
    std::map<std::string, unsigned long>::iterator i;
    if((i = pathIDs_.find(_path)) != pathIDs_.end()) {
        D123_LOGLINE(D123::TRACE, "RID = %ld", i->second);
        return i->second;
    } else {
        D123_LOG(D123::ERROR, "resource %s not loaded!", _path.c_str());
        return 0;
    }
}

//========== ResourceManager::Get
void* Core::ResourceManager::Get(const unsigned long _resourceID, const unsigned long _clssid)
{
    D123_LOG(D123::TRACE, "resource %ld requested", _resourceID);
    std::map<unsigned long, Resource*>::iterator i;
    if((i = resources_.find(_resourceID)) != resources_.end()) {
        assert(i->second);
        if(_clssid && !i->second->IsA(_clssid)) {
            D123_LOG(D123::ERROR, "requested a %ld resource, but resource %ld IsA %ld (not including base types)", _clssid, _resourceID, i->second->Clssid());
            return NULL;
        }
        if(!i->second->Loaded()) {
            D123_LOG(D123::TRACE, "loading resource %ld", _resourceID);
            i->second->Load();
        }
        return i->second->Get();
    } else {
        D123_LOG(D123::ERROR, "no resource with RID %ld", _resourceID);
        return NULL;
    }
}

//========== ResourceManager::Release
void Core::ResourceManager::Release(const unsigned long _resourceID)
{
    D123_LOG(D123::TRACE, "releasing resource %ld", _resourceID);
    std::map<unsigned long, Resource*>::iterator i;
    if((i = resources_.find(_resourceID)) != resources_.end()) {
        assert(i->second);
        if(i->second->Loaded()) {
            i->second->Release();
        }
    } else {
        D123_LOG(D123::ERROR, "no resource with RID %ld", _resourceID);
    }
}

//========== LevelLoader::DropLevel
void Core::LevelLoader::DropLevel(const int _i)
{
    if(!levels_[_i].second) return;
    _ResourceManager()->Release(levels_[_i].second);
}

//========== LevelLoader::GetLevel
Core::Level* Core::LevelLoader::GetLevel(const int _i)
{
    if(_i < 0 || _i >= levels_.size()) {
        D123_LOG(D123::ERROR, "Index %d out of range!", _i);
        throw "out of range";
    }
    if(!levels_[_i].second) levels_[_i].second = _ResourceManager()->GetRID(levels_[_i].first);
    return static_cast<Level*>(_ResourceManager()->Get(levels_[_i].second));
}
//========== Resource::Resource
Core::Resource::Resource(const std::string& _path, Core::ResourceManager* _rm)
    : path_(_path)
    , rm_(_rm)
    {}
