#include <debug/d123.h>

//========== Debug::Level
D123::LogLevel& D123::Debug::Level()
{
    static D123::LogLevel level =
        D123::TRACE;
        //D123::WARNING;
    return level;
}

//========== Debug::File
FILE*& D123::Debug::File()
{
    static FILE* logFile = stderr;
    return logFile;
}

//========== Debug::LevelFromInt
D123::LogLevel D123::Debug::LevelFromInt(const int _x)
{
    if(_x >= -2 && _x <= 3) return static_cast<D123::LogLevel>(_x);
    else if(_x > 3) return D123::TRACE;
    else return D123::FATAL;
}

//========== Debug::~Debug
D123::Debug::~Debug()
{
    fflush(File());
}
