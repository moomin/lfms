#include <string>

#ifndef LFMS_SESSION_H
#define LFMS_SESSION_H

using namespace std;

class LfmsSession {
    string name, key, status;
    bool isSubscriber;

    string error;

 public:
    LfmsSession();
    string getErrorMessage();
    string getStatus();
    string getId();

    bool restore(const string&);
    bool set(const string&, const string&, const string&, bool);
    bool save(const string&);
};

#endif
