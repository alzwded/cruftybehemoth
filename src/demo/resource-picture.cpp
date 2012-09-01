#include <demo/resource-picture.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <debug/d123.h>

//========== Resource_Picture::Load
void Demo::Resource_Picture::Load()
{
    GError* err = NULL;
    data_ = gdk_pixbuf_new_from_file(GetPath().c_str(), &err);
    if(err != NULL) {
        D123_LOG(D123::ERROR, "Unable to load resource, reason: %s", err->message);
        g_error_free(err);
        data_ = NULL;
    }
}

//========== Resource_Picture::Unload
void Demo::Resource_Picture::Release()
{
    if(data_) {
        D123_LOG(D123::INFO, "releasing %s", GetPath().c_str());
        g_object_unref(data_);
    }
}

//========== Resource_Picture::New
Core::Resource* Demo::Resource_Picture::New(
        const std::string& _path,
        Core::ResourceManager* _rm)
{
    Core::Resource* a = new Demo::Resource_Picture(_path, _rm);
    return a;
}

//========== Resource_Picture:: static init
Demo::Resource_Picture::Resource_Picture_Initializer Demo::Resource_Picture::__init__;
