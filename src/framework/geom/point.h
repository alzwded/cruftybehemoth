#ifndef GEOM_POINT_H
#define GEOM_POINT_H

#include <utility>
#include <cmath>
#include <cstddef>

namespace Geom {

//---------- Point
class Point {
public:
    //========== Point::Point
    Point(float _x =0, float _y =0)
        : p_(_x, _y)
        {}
    //========== Point::_X
    float& _X() { return p_.first; }
    //========== Point::_Y
    float& _Y() { return p_.second; }
    //========== Point::X
    float X() const { return p_.first; }
    //========== Point::Y
    float Y() const { return p_.second; }
    //========== Point::Distance
    float Distance(const Point& _point) const
    {
        float dX = (X() - _point.X());
        float dY = (Y() - _point.Y());
        return std::sqrt(dX * dX + dY * dY);
    }
    //========== Point::operator<
    bool operator<(const Point& _other) const
    {
        if(X() == _other.X()) return Y() < _other.Y();
        else return X() < _other.X();
    }
private:
    //========== Point:: private fields
    std::pair<float, float> p_;
};

} // namespace

#endif
