#ifndef CORE_BACKGROUND_H
#define CORE_BACKGROUND_H

#include <geom/point.h>
#include <cstddef>

namespace Core {

//__________ uses
class Entity;

//---------- Core::Background
class Background {
public:
    bool Intersects(const Entity& _e) const {}
    bool Intersects(const Geom::Point& _p) const {}
};

};

#endif
