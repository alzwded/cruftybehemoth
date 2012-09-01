#include <core/resource-level.h>
#include <core/resource-entity.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

//========== Resource_Level::__init__
Core::Resource_Level::Initializer Core::Resource_Level::__init__;

//========== Resource_Level::New
Core::Resource* Core::Resource_Level::New(
        const std::string& _path,
        Core::ResourceManager* _rm)
{
    _rm->_LevelLoader().PushLevel(_path);
    return new Resource_Level(_path, _rm);
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
            char* bgPath;
            char* fgPath;
            assert(4 == fscanf(f, "%d:%d:%a[^:]:%a[^\n]", &x, &y, &bgPath, &fgPath));
            // new screen
            Screen s;
            s._Background() = GetResourceManager()->GetRID(bgPath);
            s._Foreground() = GetResourceManager()->GetRID(fgPath);
            lvl_->GetEnvironment().screens_.Set(x, y, s);

            free(bgPath);
            free(fgPath);
        } else if(strncmp(type, "entity", 6) == 0) {
            D123_LOG(D123::TRACE, "loading entity");
            float x, y;
            char* path;
            assert(3 == fscanf(f, "%f:%f:%a[^\n]", &x, &y, &path));
            unsigned long rid = GetResourceManager()->GetRID(path);
            EntitySpawner* es =
                    static_cast<EntitySpawner*>(GetResourceManager()
                    ->Get(rid, Resource_Entity::CLSSID));
            if(es) {
                Core::Entity* entity = es->Spawn(x, y);
                lvl_->GetEnvironment().AddEntity(entity);
            } else {
                D123_LOG(D123::ERROR, "NULL entity spawner");
            }

            free(path);
        } else if(strncmp(type, "exit", 4) == 0) {
            D123_LOG(D123::TRACE, "loading exit");
            float x, y;
            char* nextLevel;
            assert(3 == fscanf(f, "%f:%f:%a[^\n]", &x, &y, &nextLevel));
            // :-/
            // TODO after Entity_Exit is done
            // maybe consider Entity_Trigger as a superclass?
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
