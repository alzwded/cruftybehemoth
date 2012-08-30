#ifndef CORE_BLOCKBUNDLE_H
#define CORE_BLOCKBUNDLE_H

#include <list>
#include <cstddef>

namespace Core {

//---------- Core::Block
struct Block {
    unsigned long ResourceID;
    int x, y; // relative position one to another
    static const float WIDTH = 4.0f;
    static const float HEIGHT = 4.0f;

    Block(const unsigned long _resourceID, const int _x, const int _y)
        : ResourceID(_resourceID)
        , x(_x)
        , y(_y)
        {}
};

//---------- Core::BlockBundle
class BlockBundle {
public:
    //========== BlockBundle::GetBlocks
    const std::list<Block>& GetBlocks() const
    {
        return blocks_;
    }
    //========== BlockBundle::AddBlock
    void AddBlock(const Block& _block)
    {
        blocks_.push_back(_block);
    }
private:
    std::list<Block> blocks_;
};

} // namespace

#endif
