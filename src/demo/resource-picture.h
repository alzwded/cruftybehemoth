#ifndef DEMO_RESOURCE_PICTURE_H
#define DEMO_RESOURCE_PICTURE_H

#include <core/resourcemanager.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>
#include <cstddef>

namespace Demo {

//---------- Demo::Resource_Picture
// loads a jpeg or whatever
class Resource_Picture
    : public virtual Core::Resource
{
protected:
    //========== Resource_Picture::Resource_Picture
    Resource_Picture(
            const unsigned long _rid,
            const std::string& _path,
            Core::ResourceManager* _rm)
        : Resource(_rid, _path, _rm)
        , data_(NULL)
    {}
public:
    //========== Resource_Picture::CLSSID
    static const unsigned long CLSSID = 0x80000001;
    //========== Resource_Picture::Clssid
    virtual unsigned long Clssid() const { return Resource_Picture::CLSSID; }
    //========== Resource_Picture::Load
    virtual void Load();
    //========== Resource_Picture::Load
    virtual bool Loaded() { return data_ != NULL; }
    //========== Resource_Picture::Unload
    virtual void Release();
    //========== Resource_Picture::Get
    virtual void* Get()
    {
        return data_;
    }
    //========== Resource_Picture::New
    static Resource* New(const unsigned long, const std::string& _path, Core::ResourceManager* _rm);

private:
    //========== Resource_Picture:: private fields
    GdkPixbuf* data_;

    //========== Resource_Picture:: static init
    class Resource_Picture_Initializer {
    public:
        Resource_Picture_Initializer()
        {
            Core::ResourceManager::Register("png", &Resource_Picture::New);
        }
    };
    static Resource_Picture_Initializer __init__;
};

} // namespace

#endif
