#ifndef GEOM_REGION_H
#define GEOM_REGION_H

#include <list>
#include <cstddef>
#include <geom/point.h>

namespace Geom {

class Region;   // base, abstract, sealed
class Circle;   //: Region, sealed
class Rectangle;//: Region, sealed
class Compound; //: Region, sealed

//---------- Geom::Region
class Region {
private:
    //---------- Region:: typedefs
    typedef enum { RECTANGLE, CIRCLE, COMPOSITE } RegionType;

private:
    //========== Region::Region
    Region() {}
    //========== Region::IsA
    virtual bool IsA(RegionType _region) const =0;

public:
    //========== Region::Intersects(Region)
    virtual bool Intersects(const Region&) const =0;
    //========== Region::Intersects(Point)
    virtual bool Intersects(const Point&) const =0;
    //========== Region::~Region
    virtual ~Region() {}
    //========== Region::ClonePtr
    virtual Region* ClonePtr() const =0;

    //========== Region:: friends
    friend class Circle;
    friend class Rectangle;
    friend class Compound;
};

//---------- Geom::Circle
class Circle 
    : public virtual Region
{
private:
    //========== Circle::IsA
    virtual bool IsA(const Region::RegionType _region) const { return _region == CIRCLE; }
public:
    //========== Circle::Circle
    Circle(const Point& _centre, const float _radius)
        : centre_(_centre)
        , radius_(_radius)
        {}
    //========== Circle::Intersects(Region)
    virtual bool Intersects(const Region&) const;
    //========== Circle::Intersects(Point)
    virtual bool Intersects(const Point&) const;
    //========== Circle::ClonePtr
    virtual Region* ClonePtr() const
    {
        return new Circle(*this);
    }
private:
    //========== Circle:: private fields
    Point centre_;
    float radius_;

    //========== Circle:: friends
    friend class Rectangle;
    friend class Compound;
};

//---------- Geom::Rectangle
class Rectangle
    : public virtual Region
{
private:
    //========== Rectangle::IsA
    virtual bool IsA(const Region::RegionType _region) const { return _region == RECTANGLE; }
public:
    //========== Rectangle::Rectangle
    Rectangle(const Point& _centre, const float _width, const float _height)
        : centre_(_centre)
        , width_(_width)
        , height_(_height)
        {}
    //========== Rectangle::Intersects(Region)
    virtual bool Intersects(const Region&) const;
    //========== Rectangle::Intersects(Point)
    virtual bool Intersects(const Point&) const;
    //========== Rectangle::ClonePtr
    virtual Region* ClonePtr() const
    {
        return new Rectangle(*this);
    }
private:
    //========== Rectangle:: private fields
    Point centre_;
    float width_;
    float height_;

    //========== Rectangle:: friends
    friend class Circle;
    friend class Compound;
};

//---------- Geom::Compound
class Compound
    : public virtual Region
{
private:
    //========== Compound::IsA
    virtual bool IsA(const Region::RegionType _region) const { return _region == COMPOSITE; }
public:
    //========== Compound::Compound
    Compound()
        : sealed_(false)
        {}
    //========== Compound::~Compound
    ~Compound()
    {
        for(std::list<Region*>::iterator i = regions_.begin();
                i != regions_.end();
                ++i)
        {
            delete *i;
        }
    }
    //========== Compound::AddRegion
    void AddRegion(const Region& _region)
    {
        if(sealed_) {
            throw "cannot add regions after seal!";
        }
        regions_.push_back(_region.ClonePtr());
    }
    //========== Compound::Seal
    // make read-only
    void Seal()
    {
        sealed_ = true;
    }
    //========== Compound::Intersects(Region)
    virtual bool Intersects(const Region& _region) const
    {
        for(std::list<Region*>::const_iterator i = regions_.begin();
                i != regions_.end();
                ++i)
        {
            if((*i)->Intersects(_region)) {
                return true;
            }
        }

        return false;
    }
    //========== Compound::Intersects(Point)
    virtual bool Intersects(const Point& _point) const
    {
        for(std::list<Region*>::const_iterator i = regions_.begin();
                i != regions_.end();
                ++i)
        {
            if((*i)->Intersects(_point)) {
                return true;
            }
        }
        return false;
    }
    //========== Compound::ClonePtr
    virtual Region* ClonePtr() const
    {
        return new Compound(*this);
    }
private:
    //========== Compound:: private fields
    std::list<Region*> regions_;
    bool sealed_;

    //========== Compound:: friends
    friend class Circle;
    friend class Rectangle;
};

} //namespace

#endif
