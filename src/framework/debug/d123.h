#ifndef DEBUG_D123_H
#define DEBUG_D123_H

#include <cstdio>
#include <ctime>
#include <cstddef>

namespace D123 {

//========== D123:: typedefs
typedef enum { FATAL = -2, ERROR = -1, WARNING = 0, INFO = 1, DEBUG = 2, TRACE = 3 } LogLevel;

//========== D123::Debug
struct Debug {
    static LogLevel& Level();
    static FILE*& File();
    static LogLevel LevelFromInt(const int);
    ~Debug();
};

} // namespace

#define D123_LOG(LEVEL, ...) do{\
    if(LEVEL > D123::Debug::Level()) break; \
    D123_LOGHEAD(LEVEL); \
    D123_LOGLINE(LEVEL, __VA_ARGS__); \
}while(0)

#define D123_LOGLINE(LEVEL, ...) do{\
    if(LEVEL > D123::Debug::Level()) break; \
    fprintf(D123::Debug::File(), ">    "); \
    fprintf(D123::Debug::File(), __VA_ARGS__); \
    fprintf(D123::Debug::File(), "\n"); \
}while(0)

#define D123_LOGHEAD(LEVEL) do{\
    if(LEVEL > D123::Debug::Level()) break; \
    fprintf(D123::Debug::File(), "%s|%s@%d", #LEVEL, __FILE__, __LINE__); \
    time_t t = time(NULL); \
    char tstamp[32]; \
    struct tm *tmp = localtime(&t); \
    strftime(tstamp, 32, "%c", tmp); \
    fprintf(D123::Debug::File(), "|%s", tstamp); \
    fprintf(D123::Debug::File(), "\n"); \
    fprintf(D123::Debug::File(), "> in %s\n", __PRETTY_FUNCTION__); \
}while(0)

    /*fprintf(D123::Debug::LogFile(), "%s\t%s@%d\n%s\tin: %s\n%s\t", #LEVEL, __FILE__, __LINE__, #LEVEL, __PRETTY_FUNCTION__, #LEVEL);*/ 

#endif
