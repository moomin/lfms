#include <string>
#include <cstdarg>
#include <fstream>

#ifndef LFMS_LOG_H
#define LFMS_LOG_H

class Log {
    std::ofstream file;
    short int level;
    Log *observer;

  public:
    bool init(std::string, char);
    bool log(short int, std::string, ...);
    void setLevel(short int);
    bool setObserver(Log*);
};

#endif
