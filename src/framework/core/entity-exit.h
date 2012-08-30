#ifndef CORE_ENTITY_EXIT
#define CORE_ENTITY_EXIT

#include <core/entity.h>
#include <geom/region.h>
#include <geom/point.h>
#include <core/blockbundle.h>
#include <core/level.h>
#include <cstddef>

namespace Core {

const unsigned long ENTITY_EXIT = 1;

//---------- Core::Entity_Exit
class Entity_Exit
    : public Entity
{
public:
    //========== Entity_Exit::Entity_Exit
    Entity_Exit(const Geom::Point& _location, Environment& _environment, const Environment _nextEnvironment)
        : environment_(_environment)
        , hb_(NULL)
        , nextEnvironment_(_nextEnvironment)
    {
        _GetLocation() = _location;

        _GetBlockBundle().AddBlock(Block(-1, 0, 0));
        _GetBlockBundle().AddBlock(Block(-1, 0, -1));
        _GetBlockBundle().AddBlock(Block(-1, 0, -2));
        _GetBlockBundle().AddBlock(Block(-1, 0, -3));
        _GetBlockBundle().AddBlock(Block(-1, 0, -4));
    }
    //========== Entity_Exit::~Entity_Exit
    ~Entity_Exit() {
        if(hb_) delete hb_;
    }
    //========== Entity_Exit::ClonePtr
    virtual Entity* ClonePtr() const { return new Entity_Exit(*this); }
    //========== Entity_Exit::OnColision
    virtual void OnCollision(Entity& _entity)
    {
        // TODO figure out how to tell the game to display the next environment
        environment_.GetLevel()->GetEnvironment() = nextEnvironment_;
    }
    //========== Entity_Exit::Clssid
    virtual unsigned long Clssid() const { return ENTITY_EXIT; }
    //========== Entity_Exit::IsHitBoxDamaging
    virtual bool IsHitBoxDamaging() const { return false; }
    //========== Entity_Exit::HitBox
    virtual const Geom::Region& HitBox() const
    {
    /*
        +-+
 y0 + 0 | |
        +-+
      1 | |
        +-+
      2 |D|  D = desired location of hit box's centre
        +-+
      3 | |
        +-+
      4 |P|  P = location_ member
        +-+
     x = 0
    */
        const_cast<Entity_Exit*>(this)->hb_ = new Geom::Rectangle(
            Geom::Point(GetLocation().X(),
                GetLocation().Y() - 2 * Block::HEIGHT / 2),
            Block::WIDTH,
            5 * Block::HEIGHT);
        return *hb_;
    }
    //========== Entity_Exit::Loop
    virtual void Loop(const Environment& _E) { return; }
private:
    //========== Entity_Exit:: private fields
    Environment& environment_;
    Environment nextEnvironment_;
    Geom::Region* hb_;
};

} //namespace

#endif
