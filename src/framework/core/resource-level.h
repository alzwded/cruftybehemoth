#ifndef CORE_RESOURCE_LEVEL_H
#define CORE_RESOURCE_LEVEL_H

#include <core/resourcemanager.h>
#include <core/level.h>
#include <debug/d123.h>
#include <string>
#include <cstddef>

namespace Core {

//---------- Core::Resource_Level
class Resource_Level
    : public virtual Core::Resource
{
public:
    //========== Resource_Level::Resource_Level
    Resource_Level(
            const std::string& _path,
            Core::ResourceManager* _rm)
        : Resource(_path, _rm)
        , lvl_(NULL)
        {}
    //========== Resource_Level::CLSSID
    static const unsigned long CLSSID = 0x10;
    //========== Resource_Level::Clssid
    virtual unsigned long Clssid() const { return Resource_Level::CLSSID; }
    //========== Resource_Level::Load
    virtual void Load();
    //========== Resource_Level::Loaded
    // TODO move this to abstract class
    virtual bool Loaded() { return lvl_ != NULL; }
    //========== Resource_Level::Release
    // TODO move this to the abstract class
    virtual void Release();
    //========== Resource_Level::Get
    // TODO move this to abstract class
    virtual void* Get() { return lvl_; }
    //========== Resource_Level::New
    static Resource* New(const std::string& _path, Core::ResourceManager* _rm);
private:
    //========== Resource_Level:: private fields
    // TODO move this to abstract class as void* data_;
    Level* lvl_;

    //========== Resource_Level:: static initializer
    struct Initializer {
        Initializer() {
            ResourceManager::Register("level", &Resource_Level::New);
        }
    };
    static Initializer __init__;
};

} // namespace

#endif
