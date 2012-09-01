#ifndef CORE_SCREEN_H
#define CORE_SCREEN_H

#include <set>
#include <list>
#include <utility>
#include <cstddef>
#include <geom/point.h>
#include <geom/region.h>
#include <core/entity.h>
#include <core/util.h>

#define CORE_SCREEN_WIDTH (320.0f)
#define CORE_SCREEN_HEIGHT (200.0f)

namespace Core {

//__________ uses
class Game;
class Level;
class Environment;
class ScreenBucketTree;
class Resource_Level;

//---------- Core::Screen
class Screen {
public:
    //========== Screen:: typedefs
    typedef enum { QUADRANT1, QUADRANT2, QUADRANT3, QUADRANT4 } Quadrant;
    //========== Screen::Screen
    Screen()
        : origin_(0, 0)
        , extent_(Geom::Point(0., 0.), CORE_SCREEN_WIDTH, CORE_SCREEN_HEIGHT)
        , left_(NULL)
        , right_(NULL)
        , above_(NULL)
        , below_(NULL)
        {}
    //========== Screen::~Screen
    ~Screen()
    {
        for(std::set<Entity*, DerefLess<Entity> >::iterator i = entities_.begin();
                i != entities_.end();
                ++i)
        {
            delete (*i);
        }
    }
    //========== Screen::HitWall(Entity)
    bool HitWall(const Entity& _e) const
    {
        //return bg_.Intersects(_e.GetLocation());
        // TODO walls for a screen
    }
    //========== Screen::HitWall(Point)
    bool HitWall(const Geom::Point& _p) const
    {
        //return bg_.Intersects(_p);
        // TODO walls for a screen
    }
    //========== Screen::GetScreenQuadrant
    Quadrant GetScreenQuadrant(const Geom::Point& _p) const
    {
        Geom::Point c(origin_.first * CORE_SCREEN_WIDTH, origin_.second * CORE_SCREEN_HEIGHT);
        if(_p.X() >= c.X() && _p.Y() >= c.Y()) return Screen::QUADRANT1;
        if(_p.X() < c.X() && _p.Y() >= c.Y()) return Screen::QUADRANT2;
        if(_p.X() < c.X() && _p.Y() < c.Y()) return Screen::QUADRANT3;
        if(_p.X() >= c.X() && _p.Y() < c.Y()) return Screen::QUADRANT3;
        return Screen::QUADRANT1;
    }
    //========== Screen::Left
    Screen* Left() const { return left_; }
    //========== Screen::Right
    Screen* Right() const { return right_; }
    //========== Screen::Above
    Screen* Above() const { return above_; }
    //========== Screen::Below
    Screen* Below() const { return below_; }

    //========== Screen::GetEntities
    const std::set<Entity*, DerefLess<Entity> >& GetEntities() const { return entities_; }
    //========== Screen::Background
    const unsigned long Background() const { return bg_; }
    //========== Screen::Foreground
    const unsigned long Foreground() const { return fg_; }

    //========== Screen::Extent
    const Geom::Region& Extent() const {
        return dynamic_cast<const Geom::Region&>(extent_);
    }

    //========== Screen::PointOnScreen
    bool PointOnScreen(const Geom::Point& _p) { return Extent().Intersects(_p); }
    //========== Screen::RegionOnScreen
    bool RegionOnScreen(const Geom::Region& _r) { return Extent().Intersects(_r); }
private:
    //========== Screen::SetLeft
    void SetLeft(Screen* _s) { left_ = _s; }
    //========== Screen::SetRight
    void SetRight(Screen* _s) { right_ = _s; }
    //========== Screen::SetAbove
    void SetAbove(Screen* _s) { above_ = _s; }
    //========== Screen::SetBelow
    void SetBelow(Screen* _s) { below_ = _s; }

    //========== Screen::AddEntity
    void AddEntity(Entity* _entity) { entities_.insert(_entity); }
    //========== Screen::RemoveEntity
    Entity* RemoveEntity(Entity* _entity)
    {
        if(!_entity) return _entity;
        entities_.erase(_entity);
    }

    //========== Screen::_Background
    unsigned long& _Background() { return bg_; }
    //========== Screen::_Foreground
    unsigned long& _Foreground() { return fg_; }
    //========== Screen::SetOriginBlock
    void SetOriginBlock(const std::pair<int, int>& _p)
    {
        origin_ = _p;
        extent_ = Geom::Rectangle(
            Geom::Point(_p.first * CORE_SCREEN_WIDTH, _p.second * CORE_SCREEN_HEIGHT),
            CORE_SCREEN_WIDTH,
            CORE_SCREEN_HEIGHT);
    }
private:
    //========== Screen:: private fields
    Screen* left_;
    Screen* right_;
    Screen* above_;
    Screen* below_;
    std::set<Entity*, DerefLess<Entity> > entities_;
    unsigned long bg_, fg_;
    std::pair<int, int> origin_;
    Geom::Rectangle extent_;

    //========== Screen:: friends
    friend class Core::ScreenBucketTree;
    friend class Core::Environment;
    friend class Core::Game;
    friend class Core::Resource_Level;
    friend class Core::Level;
};

//__________ typedefs
typedef std::list<Screen*> ScreenList;

} // namespace

#endif
