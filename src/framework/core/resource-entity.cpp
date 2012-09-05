#include <core/resource-entity.h>
#include <debug/d123.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

//========== Resource_Entity::Load
void Core::Resource_Entity::Load()
{
    FILE* f = fopen(GetPath().c_str(), "r");
    if(!f) {
        D123_LOG(D123::ERROR, "Could not load resource from %s", GetPath().c_str());
    }
    es_ = NewEntitySpawner();
    // read initial velocity, graphics blocks, (sound resources TODO)
    float f1, f2;
    assert(2 == fscanf(f, "%f%f", &f1, &f2));
    fscanf(f, "%*[\n]");
    es_->initialvelocity_ = Geom::Point(f1, f2);
    char* s;
    bool loop = false;
    Core::BlockBundle b;
    do { // FIXME loop is bad
        if(feof(f)) break; // FIXME read \n AFTER, not before the loop
        fscanf(f, "%a[^\n]", &s);
        fscanf(f, "%*[\n]");
        if(strlen(s) >= 2 && s[0] == '=' && s[1] == '=') loop = false;
        int x, y;
        char* path;
        assert(sscanf(s, "%d %d %a[^\n]", &x, &y, &path) == 3);
        b.AddBlock(Core::Block(GetResourceManager()->GetRID(path), x, y));
        free(s);
        free(path);
    }while(loop);
    es_->blocks_ = b;

    // call private _Load follows
    _Load(f, es_);
    fclose(f);
}

//========== Resource_Entity::Resource_Entity
Core::Resource_Entity::Resource_Entity(const unsigned long _rid, const std::string& _path, Core::ResourceManager* _rm)
    : ::Core::Resource(_rid, _path, _rm)
    , es_(NULL)
    {}

//========== Resource_Entity::Release
void Core::Resource_Entity::Release()
{
    delete es_;
}
