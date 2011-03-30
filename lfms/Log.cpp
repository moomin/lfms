#include <string>
#include <cstdarg>
#include "Log.h"

Log::init(string file, char mode)
{
  Log::file.open(resolve_path(file).c_str(), ios_base::);
}

Log::log(short int level, string format, ...)
{

}

Log::setLevel(short int level)
{
    Log::level = level;
}

Log::setObserver(Log *observer)
{
    Log::observer = observer;
    return true;
}



