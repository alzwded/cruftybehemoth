#include <demo/entity-demo.h>

//========== Entity_Demo::Loop
void Demo::Entity_Demo::Loop(const Core::Environment& _e)
{
    if(accelerating_) {
        if(GetVelocity().X() > 0.f) {
            _GetVelocity()._X() = _GetVelocity()._X() + D2X;
        } else if(GetVelocity().X() < 0.f) {
            _GetVelocity()._X() = _GetVelocity()._X() - D2X;
        } else if(GetVelocity().Y() > 0.f) {
            _GetVelocity()._Y() = _GetVelocity()._Y() + D2Y;
        } else if(GetVelocity().Y() < 0.f) {
            _GetVelocity()._Y() = _GetVelocity()._Y() - D2Y;
        }
    } else {
        if(GetVelocity().X() > 0.f) {
            _GetVelocity()._X() = _GetVelocity()._X() - D2X;
            if(GetVelocity().X() < 0.f) _GetVelocity()._X() = 0.f;
        } else if(GetVelocity().X() < 0.f) {
            _GetVelocity()._X() = _GetVelocity()._X() + D2X;
            if(GetVelocity().X() > 0.f) _GetVelocity()._X() = 0.f;
        } else if(GetVelocity().Y() > 0.f) {
            _GetVelocity()._Y() = _GetVelocity()._Y() - D2Y;
            if(GetVelocity().Y() < 0.f) _GetVelocity()._Y() = 0.f;
        } else if(GetVelocity().Y() < 0.f) {
            _GetVelocity()._Y() = _GetVelocity()._Y() + D2Y;
            if(GetVelocity().Y() > 0.f) _GetVelocity()._Y() = 0.f;
        }
    }
}

//========== Entity_Demo::HitBox
const Geom::Region& Demo::Entity_Demo::HitBox() const 
{
    Geom::Rectangle* r = new Geom::Rectangle(GetLocation(), 8.0f, 8.0f);
    return *r;
}

//========== Entity_Demo::IsHitBoxDamaging
bool Demo::Entity_Demo::IsHitBoxDamaging() const
{
    return false;
}

//========== Entity_Demo::OnCollision
void Demo::Entity_Demo::OnCollision(const Core::Entity& _e, const Geom::Point& _p)
{
    D123_LOG(D123::INFO, "Collided with %ld at point (%f,%f)", _e.ID(), _p.X(), _p.Y());
}

//========== Entity_Demo::ClonePtr
Core::Entity* Demo::Entity_Demo::ClonePtr() const
{
    Demo::Entity_Demo* e = new Demo::Entity_Demo(GetLocation());
    e->accelerating_ = accelerating_;
    e->_GetVelocity() = GetVelocity();
    e->_GetBlockBundle() = GetBlockBundle();
    return e;
}
