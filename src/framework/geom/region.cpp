#include <geom/region.h>
#include <geom/point.h>
#include <geom/util.h>

//========== Circle::Intersects(Region)
bool Geom::Circle::Intersects(const Geom::Region& _region) const
{
    if(_region.IsA(Geom::Region::CIRCLE)) {
        const Geom::Circle& other = dynamic_cast<const Geom::Circle&>(_region);

        if(_region.Intersects(centre_)) return true;

        return centre_.Distance(other.centre_) < (radius_ + other.radius_);
    } else if(_region.IsA(Geom::Region::RECTANGLE)) {
        const Geom::Rectangle& other =
                dynamic_cast<const Geom::Rectangle&>(_region);

        if(_region.Intersects(centre_)) return true;

        Geom::Rectangle boundingBox(centre_, radius_ * 2, radius_ * 2);
        if(other.Intersects(boundingBox)) {
            Geom::Point topLeft(
                    other.centre_.X() - other.width_ / 2,
                    other.centre_.Y() - other.height_ / 2);
            if(centre_.X() < topLeft.X() && centre_.Y() < topLeft.Y()
                    && !Intersects(topLeft))
                return false;

            Geom::Point topRight(
                    other.centre_.X() + other.width_ / 2,
                    other.centre_.Y() - other.height_ / 2);
            if(centre_.X() > topRight.X() && centre_.Y() < topLeft.Y()
                    && !Intersects(topRight))
                return false;

            Geom::Point bottomRight(
                    other.centre_.X() + other.width_ / 2,
                    other.centre_.Y() + other.height_ / 2);
            if(centre_.X() > bottomRight.X() && centre_.Y() > bottomRight.Y()
                    && !Intersects(bottomRight))
                return false;

            Geom::Point bottomLeft(
                    other.centre_.X() - other.width_ / 2,
                    other.centre_.Y() + other.height_ / 2);
            if(centre_.X() < bottomLeft.X() && centre_.Y() > bottomLeft.Y()
                    && !Intersects(bottomLeft))
                return false;

            return true;
        } else {
            return false;
        }
    } else if(_region.IsA(Geom::Region::COMPOSITE)) {
        return _region.Intersects(*this);
    } else {
        throw "someone tampered with this class";
    }
}

//========== Rectangle::Intersects(Region)
bool Geom::Rectangle::Intersects(const Geom::Region& _region) const
{
    if(_region.IsA(Geom::Region::RECTANGLE)) {
        const Geom::Rectangle& other =
                dynamic_cast<const Geom::Rectangle&>(_region);

        Geom::Point topLeft(
                other.centre_.X() - other.width_ / 2,
                other.centre_.Y() - other.height_ / 2);
        if(Intersects(topLeft)) return true;

        Geom::Point topRight(
                other.centre_.X() + other.width_ / 2,
                other.centre_.Y() - other.height_ / 2);
        if(Intersects(topRight)) return true;

        Geom::Point bottomRight(
                other.centre_.X() + other.width_ / 2,
                other.centre_.Y() + other.height_ / 2);
        if(Intersects(bottomRight)) return true;

        Geom::Point bottomLeft(
                other.centre_.X() - other.width_ / 2,
                other.centre_.Y() + other.height_ / 2);
        if(Intersects(bottomLeft)) return true;

        // check this rectangle is fully interior:
        Geom::Point randomPoint(
                centre_.X() - width_ / 2,
                centre_.Y() - height_ / 2);
        if(other.Intersects(randomPoint)) return true;

        return false;
    } else if(_region.IsA(Geom::Region::CIRCLE)) {
        return _region.Intersects(*this);
    } else if(_region.IsA(Geom::Region::COMPOSITE)) {
        return _region.Intersects(*this);
    } else {
        throw "someone tampered with this class";
    }
}

//========== Circle::Intersects(Point)
inline bool Geom::Circle::Intersects(const Geom::Point& _point) const
{
    return centre_.Distance(_point) < radius_;
}

//========== Rectangle::Intersects(Point)
inline bool Geom::Rectangle::Intersects(const Geom::Point& _point) const
{
    return _point.X() >= (centre_.X() - width_ / 2)
            && _point.X() < (centre_.X() + width_ / 2)
            && _point.Y() >= (centre_.Y() - height_ / 2)
            && _point.Y() < (centre_.Y() + height_ / 2);
}
