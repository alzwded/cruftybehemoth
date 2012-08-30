#ifndef CORE_BUCKEScreenScreenREE_H
#define CORE_BUCKEScreenScreenREE_H

#include <map>
#include <geom/point.h>
#include <core/screen.h>
#include <cstddef>

namespace Core {

//---------- Core::BucketScreenTree
class ScreenBucketTree {
public:
    //========== BucketScreenTree::Set
    void Set(const int _x, const int _y, const Screen& _screen)
    {
        std::map<int, std::map<int, Screen> >::iterator ix;
        std::map<int, Screen>::iterator iy;

        screens_[_x][_y] = _screen;
        Screen& screen = Get(_x, _y);

        if((ix = screens_.find(_x - 1)) != screens_.end()
                && (iy = ix->second.find(_y)) != ix->second.end())
        {
            screen.SetLeft(&(iy->second));
        } else {
            screen.SetLeft(NULL);
        }

        if((ix = screens_.find(_x)) != screens_.end()
                && (iy = ix->second.find(_y + 1)) != ix->second.end())
        {
            screen.SetBelow(&(iy->second));
        } else {
            screen.SetBelow(NULL);
        }

        if((ix = screens_.find(_x + 1)) != screens_.end()
                && (iy = ix->second.find(_y)) != ix->second.end())
        {
            screen.SetRight(&(iy->second));
        } else {
            screen.SetRight(NULL);
        }

        if((ix = screens_.find(_x)) != screens_.end()
                && (iy = ix->second.find(_y - 1)) != ix->second.end())
        {
            screen.SetAbove(&(iy->second));
        } else {
            screen.SetAbove(NULL);
        }
    }
    //========== BucketScreenTree::Get
    Screen& Get(const int _x, const int _y)
    {
        return screens_[_x][_y];
    }

public:
    //========== BucketScreenTree::FindFor
    Screen* FindFor(const Geom::Point& _point)
    {
        int x = _point.X() / CORE_SCREEN_WIDTH;
        int y = _point.Y() / CORE_SCREEN_HEIGHT;
        std::map<int, std::map<int, Screen> >::iterator ix;
        std::map<int, Screen>::iterator iy;

        if((ix = screens_.find(x)) != screens_.end()
                && (iy = ix->second.find(y)) != ix->second.end())
        {
            return &(iy->second);
        }

        return NULL;
    }
    //========== BucketScreenTree::Clear
    void Clear() { screens_.clear(); }

private:
    //========== BucketScreenTree:: private fields
    std::map<int, std::map<int, Screen> > screens_;
};

} // namespace

#endif
