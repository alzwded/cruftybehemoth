#ifndef DEMO_ENTITY_DEMO_H
#define DEMO_ENTITY_DEMO_H

#include <core/entity.h>
#include <geom/point.h>

namespace Demo {

class EntitySpawner;

//---------- Demo::Entity_Demo
class Entity_Demo
    : public virtual Core::Entity
{
protected:
    static const float D2X = 2.0f;
    static const float D2Y = 2.0f;
public:
    //========== Entity_Demo::Entity_Demo
    Entity_Demo(const Geom::Point& _p)
        : Core::Entity(_p)
        , accelerating_(0)
        {}
    //========== Entity_Demo::Loop
    virtual void Loop(const Core::Environment&);
    //========== Entity_Demo::HitBox
    virtual const Geom::Region& HitBox() const;
    //========== Entity_Demo::IsHitBoxDamaging
    virtual bool IsHitBoxDamaging() const;
    //========== Entity_Demo::CLSSID
    static const unsigned long CLSSID = 0x80000200;
    //========== Entity_Demo::Clssid
    virtual unsigned long Clssid() const { return CLSSID; }
    //========== Entity_Demo::OnCollision
    virtual void OnCollision(const Core::Entity&, const Geom::Point&);
    //========== Entity_Demo::ClonePtr
    virtual Core::Entity* ClonePtr() const;

private:
    //========== Entity_Demo:: private fields
    int accelerating_;

    //========== Entity_Demo:: friends
    friend class Demo::EntitySpawner;
};

} // namespace

#endif