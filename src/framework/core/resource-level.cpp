#include <core/resource-level.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

//========== Resource_Level::__init__
Core::Resource_Level::Initializer Core::Resource_Level::__init__;

//========== Resource_Level::New
Core::Resource* Core::Resource_Level::New(const std::string& _path)
{
    return new Resource_Level(_path);
}

//========== Resource_Level::Load
void Core::Resource_Level::Load()
{
    // open file
    // read %a[^:]:, switch type, read (%a[^:\n]){type::N}
    // close file
    FILE* f = fopen(GetPath().c_str(), "r");
    if(!f) {
        D123_LOG(D123::ERROR, "cannot load level at path %s", GetPath().c_str());
        return;
    }
    lvl_ = Core::Level::New();
    while(!feof(f)) {
        char* type;
        fscanf(f, "%a[^:]:", &type);
        if(strncmp(type, "screen", 6) == 0) {
            D123_LOG(D123::TRACE, "loading screen");
            int x, y;
            char* bg;
            char* fg;
            assert(4 == fscanf(f, "%d:%d:%a[^:]:%a[^\n]", &x, &y, &bg, &fg));
            // new screen
            free(bg);
            free(fg);
        } else if(strncmp(type, "entity", 6) == 0) {
            D123_LOG(D123::TRACE, "loading entity");
            float x, y;
            char* path;
            assert(3 == fscanf(f, "%f:%f:%a[^\n]", &x, &y, &path));
            // screens->AddEntity(registry_[path]->Get()->Spawn(x, y));
            free(path);
        } else if(strncmp(type, "exit", 4) == 0) {
            D123_LOG(D123::TRACE, "loading exit");
            float x, y;
            char* nextLevel;
            assert(3 == fscanf(f, "%f:%f:%a[^\n]", &x, &y, &nextLevel));
            // :-/
            free(nextLevel);
        }
        free(type);
    }
    fclose(f);
}

//========== Resource_Level::Release
void Core::Resource_Level::Release()
{
    if(lvl_) delete lvl_;
}
