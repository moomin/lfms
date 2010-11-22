#ifndef LFMS_H
#define LFMS_H

#define LFMS_VERSION "0.1"

#define LFMS_API_URL "http://ws.audioscrobbler.com/2.0/"
#define LFMS_API_KEY "98c7d501c9c52f770dc25a63df6ce7ec"
#define LFMS_API_SECRET "35431b8abe8861a454ed34286033c148"

#include "LfmsConfig.h"
#include "LfmsSession.h"
#include "LfmsWsApi.h"
#include "LfmsTrack.h"
#include "helpers.h"

class Lfms {
    LfmsConfig cfg;
    LfmsSession session;
    LfmsWsApi api;

    bool readConfig();
 public:
    bool init(int, char*[]);
    bool readConfig(int, char*[]);
    bool initSession(bool forceNew = false);
    bool action();
    bool nowPlaying();
    bool fillTrackInfo(LfmsTrack& track, arrStr&);
};

#endif
