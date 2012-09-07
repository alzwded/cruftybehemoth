#include <core/time.h>

//========== Core::gettime
void Core::Time::gettime(Core::Time::core_time_t& t)
{
    clock_gettime(CLOCK_REALTIME, &t);
}

//========== Core::diff
Core::Time::core_time_t Core::Time::diff(const Core::Time::core_time_t& start, const Core::Time::core_time_t& end)
{
    Core::Time::core_time_t d;
    if((end.tv_nsec - start.tv_nsec) < 0) {
        d.tv_sec = end.tv_sec - start.tv_sec - 1;
        d.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        d.tv_sec = end.tv_sec - start.tv_sec;
        d.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return d;
}

//========== Core::remainingMS(diff, long)
long Core::Time::remainingMS(const Core::Time::core_time_t& d, const long frameSpeed)
{
    long ret = frameSpeed - d.tv_sec * 1000 + d.tv_nsec / 1000000;

    return ret;
}

//========== Core::remainingMS(start, end, long)
long Core::Time::remainingMS(const Core::Time::core_time_t& start, const Core::Time::core_time_t& end, const long frameSpeed)
{
    Core::Time::core_time_t d = Core::Time::diff(start, end);

    return remainingMS(d, frameSpeed);
}
