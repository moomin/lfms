#include <string>

#ifndef LFMS_SESSION_H
#define LFMS_SESSION_H

using namespace std;

class LfmsSession {
    string name, key;
    bool isSubscriber;

    string error;

 public:
    string getErrorMessage();

    int restore(const string&);
    int save(const string&);
};

#endif
