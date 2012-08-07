#include <string>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include "helpers.h"
#include "Log.h"

using namespace std;

/*
 * @TODO
 * - add support for many observers
 * - loong-time todo: add support for different log backends
 */

Log::Log()
{
  console = false;
  level = LOG_INFO;

  levelName[LOG_EMERG]   = "[emerg]";
  levelName[LOG_ALERT]   = "[alert]";
  levelName[LOG_CRIT]    = "[crit ]";
  levelName[LOG_ERR]     = "[error]";
  levelName[LOG_WARNING] = "[warn ]";
  levelName[LOG_NOTICE]  = "[notice]";
  levelName[LOG_INFO]    = "[info ]";
  levelName[LOG_DEBUG]   = "[debug]";
}

bool Log::init(string file, char mode)
{
    Log::file.open(resolve_path(file).c_str(), ios_base::out | ios_base::app);
    observer = 0;

    return Log::file.is_open();
}

void Log::notifyObservers(short int level, string format, va_list ap)
{
    if (observer)
    {
        observer->log(level, format, ap);
    }
}

bool Log::log(short int level, string format, va_list ap)
{
    char *msg = NULL;

    if (level > Log::level)
    {
        notifyObservers(level, format, ap);
        return true;
    }

    format = " " + format;

    if(vasprintf(&msg, format.c_str(), ap) >= 0)
    {
        time_t rawtime;
        struct tm *time;
        char timebuf[256];

        std::time(&rawtime);
        time = gmtime(&rawtime);

        strftime(timebuf, 255, "%F %T %Z ", time);

        //log to file
        file << timebuf << levelName[level] << msg << "\n";

        //optionally log to console
        if (console && (level <= LOG_ERR))
        {
            cerr << timebuf << levelName[level] <<msg << "\n";
        }
        else if (console)
        {
            cout << timebuf << levelName[level] << msg << "\n";
        }

        free(msg);
    }
    else
    {
        return false;
    }
  
    return true;
}

bool Log::log(short int level, string format, ...)
{
    va_list ap;
    bool result;

    va_start(ap, format);
    result = log(level, format, ap);
    va_end(ap);

    return result;
}

bool Log::setObserver(Log *observer)
{
    Log::observer = observer;
    return true;
}

Log::~Log()
{
  file.close();
}
