#include <string>
#include <cstdarg>
#include "helpers.h"
#include "Log.h"

using namespace std;

bool Log::init(string file, char mode)
{
    Log::file.open(resolve_path(file).c_str(), ios_base::out | ios_base::app);
    observer = 0;

    return Log::file.is_open();
}

bool Log::log(short int level, string format)
{
    file << format.c_str() << "\n";

    if (observer)
    {
        observer->log(level, format);
    }
    file.close();
    return true;
}

void Log::setLevel(short int level)
{
    Log::level = level;
}

bool Log::setObserver(Log *observer)
{
    Log::observer = observer;
    return true;
}



