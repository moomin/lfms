#include <string>
#include "helpers.h"

#ifndef LFMS_TRACK_H
#define LFMS_TRACK_H

class LfmsTrack {

 public:
    std::string track;
    int timestamp;
    std::string artist;
    std::string album;
    std::string albumArtist;
    std::string context;
    std::string streamId;
    int trackNumber;
    std::string mbid;
    int duration;
};

#endif
