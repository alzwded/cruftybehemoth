#ifndef CORE_TIME_H
#define CORE_TIME_H

#include <time.h>

namespace Core {

namespace Time {
    typedef timespec core_time_t;

    void gettime(core_time_t& t);
    core_time_t diff(const core_time_t& start, const core_time_t& end);
    long remainingMS(const core_time_t& start, const core_time_t& end, const long frameSpeed);
    long remainingMS(const core_time_t& t, const long frameSpeed);
} // namespace Time

} // namespace Core

#endif
