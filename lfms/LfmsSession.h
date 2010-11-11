#include <string>
#include "LfmsConfig.h"

#ifndef LFMS_SESSION_H
#define LFMS_SESSION_H

using namespace std;

class LfmsSession {
    LfmsConfig config;
    bool isActive;
    string status, id;
    string nowPlayingUrl;
    string submissionUrl;

    string error;

 public:
    LfmsSession();
    int init(LfmsConfig cfg);
    string getErrorMessage();

    int handshake();
    int restore();
    int open();
    int save();

};

#endif
