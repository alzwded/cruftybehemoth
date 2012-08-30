#include <core/environment.h>
#include <core/screen.h>
#include <core/entity.h>
#include <core/entity-exit.h>
#include <core/screenbuckettree.h>
#include <geom/point.h>
#include <debug/d123.h>
#include <map>
#include <algorithm>
#include <stdio.h>

//========== Environment::GetEntitiesNear
Core::EntityList Core::Environment::GetEntitiesNear(const Entity& _entity)
{
    /*std::map<Geom::Point, EntityList>::const_iterator i =
            C_entities_.find(_entity.GetLocation());
    if(i != C_entities_.end()) 
    {
        return i->second;
    }*/
    if(!C_entities_.empty()) return C_entities_;

    Core::EntityList ret;
    Core::ScreenList activeScreens = GetScreensFor(_entity.GetLocation());

    for(Core::ScreenList::iterator i = activeScreens.begin();
            i != activeScreens.end();
            ++i)
    {
        for(Core::EntitySet::iterator j = (*i)->GetEntities().begin();
                j != (*i)->GetEntities().end();
                ++j)
        {
            ret.push_back(*j);
        }
    }

    //C_entities_[_entity.GetLocation()] = ret;
    const_cast<Environment*>(this)->C_entities_ = ret;
    return ret;
}

//========== Environment::GetScreensFor(Point)
Core::ScreenList Core::Environment::GetScreensFor(const Geom::Point& _location)
{
    /*std::map<Geom::Point, Core::ScreenList>::const_iterator i =
            C_screens_.find(_location);
    if(i != C_screens_.end())
    {
        return i->second;
    }*/
    if(!C_screens_.empty()) return C_screens_;

    Core::ScreenList ret;

    Core::Screen* s = screens_.FindFor(_location);
    if(!s) return ret;

    ret.push_back(s);

    switch(s->GetScreenQuadrant(_location)) {
    case Core::Screen::QUADRANT1:
        if(s->Above()) ret.push_back(s->Above());
        if(s->Above() && s->Above()->Right()) ret.push_back(s->Above()->Right());
        if(s->Right()) ret.push_back(s->Right());
        break;
    case Core::Screen::QUADRANT4:
        if(s->Right()) ret.push_back(s->Right());
        if(s->Right() && s->Right()->Below()) ret.push_back(s->Right()->Below());
        if(s->Below()) ret.push_back(s->Below());
    case Core::Screen::QUADRANT3:
        if(s->Below()) ret.push_back(s->Below());
        if(s->Below() && s->Below()->Left()) ret.push_back(s->Below()->Left());
        if(s->Left()) ret.push_back(s->Left());
    case Core::Screen::QUADRANT2:
        if(s->Left()) ret.push_back(s->Left());
        if(s->Left() && s->Left()->Above()) ret.push_back(s->Left()->Above());
        if(s->Above()) ret.push_back(s->Above());
    }
    //const_cast<Environment*>(this)->C_screens_[_location] = ret;
    const_cast<Environment*>(this)->C_screens_ = ret;
    return ret;
}

//========== Environment::GetScreensFor
Core::ScreenList Core::Environment::GetScreensFor(const Core::Entity& _entity)
{
    return GetScreensFor(_entity.GetLocation());
}

//========== Environment::UpdateLocation
inline void Core::Environment::UpdateLocation(
        Core::Entity& _entity,
        const Geom::Point& _oldLocation)
{
    Screen* s1 = screens_.FindFor(_oldLocation);
    Screen* s2 = screens_.FindFor(_entity.GetLocation());

    if(s2->HitWall(_entity)) {
        if(s1->HitWall(_oldLocation)) {
            _entity.location_ = _oldLocation;
        }
        // TODO implement glitch nicely
        Geom::Point orig(_entity.location_);
        for(float i = 1.; i <= 3. * std::max(_entity.velocity_.X(), _entity.velocity_.Y()); i += 1.) {
            _entity.location_ = orig;
            _entity.location_._X() -= i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_._X() += 2 * i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_ = orig;
            _entity.location_._Y() -= i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_._Y() += 2 * i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_._X() += i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_._X() -= 2 * i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_._Y() -= 2 * i;
            if(!s2->HitWall(_entity)) break;
            _entity.location_._X() += 2 * i;
            if(!s2->HitWall(_entity)) break;
            D123_LOG(D123::ERROR, "entity %ld with clssid %ld got stuck in a waill\n", _entity.ID(), _entity.Clssid());
            break;
        }
    }

    if(s1 != (s2 = screens_.FindFor(_entity.GetLocation())))
    {
        if(s1 != NULL) s1->RemoveEntity(_entity);
        if(s2 != NULL) s2->AddEntity(_entity);
    }

    return;
}

//========== Environment::ClearCache
void Core::Environment::ClearCache()
{
    C_entities_.clear();
    C_screens_.clear();
}
