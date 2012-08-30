#ifndef CORE_DISPLAYADAPTER_H
#define CORE_DISPLAYADAPTER_H

#include <core/screen.h>
#include <core/resourcemanager.h>
#include <cstddef>

namespace Core {

class DisplayAdapter {
public:
    virtual void Render(const Core::ScreenList&, const Core::ResourceManager&) =0;
};

} // namespace

#endif
