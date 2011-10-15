#ifndef LFMS_H
#define LFMS_H

#define LFMS_VERSION "0.1"

#define LFMS_API_URL "http://ws.audioscrobbler.com/2.0/"
#define LFMS_API_KEY "98c7d501c9c52f770dc25a63df6ce7ec"
#define LFMS_API_SECRET "35431b8abe8861a454ed34286033c148"

#define LFMS_MAX_SCROBBLE_TRIES 4

#include "LfmsConfig.h"
#include "LfmsSession.h"
#include "LfmsWsApi.h"
#include "LfmsTrack.h"
#include "Log.h"
#include "helpers.h"

class Lfms {
    LfmsConfig cfg;
    LfmsSession session;
    Log log;
    Log socketLog;
    LfmsWsApi api;

    bool readConfig(int, char*[]);
 public:
    bool init(int, char*[]);
    bool initSession(bool forceNew = false);
    bool submitQueue();
    void logApiError(const LfmsTrack&, const char*);
    void logApiSuccess(const LfmsTrack&, const char*);
    bool action();
    bool nowPlaying(short int tryNumber = 1);
    bool scrobble(short int tryNumber = 1);
    bool fillTrackInfo(LfmsTrack& track, arrStr&);
    bool putTrackIntoQueue(LfmsTrack& track);
};

#endif
