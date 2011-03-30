#include <string>
#include <cstdarg>

class Log {
    ofstream file;
    short int level;
    Log *observer;

  public:
    init(string, char);
    log(short int, string, ...);
    setLevel(short int);
    setObserver(Log*);
}
