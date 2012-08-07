#include <string>
#include <cstdarg>
#include <fstream>
#include <syslog.h>

#ifndef LFMS_LOG_H
#define LFMS_LOG_H

class Log {
    std::ofstream file;
    Log *observer;
    arrIndex levelName;

  public:
    bool console;
    short int level;

    Log();
    bool init(std::string, char);
    void notifyObservers(short int, std::string, va_list);
    bool log(short int, std::string, va_list);
    bool log(short int, std::string, ...);
    bool setObserver(Log*);
    ~Log();
};

#endif
