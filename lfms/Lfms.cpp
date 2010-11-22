#include <cstdio>
#include <cstdlib>
#include <time.h>

#include "Lfms.h"
#include "LfmsTrack.h"
#include "helpers.h"

bool Lfms::init(int argc, char* argv[])
{
    api.setAccountInfo(LFMS_API_KEY, LFMS_API_SECRET);
    api.setServiceInfo(LFMS_API_URL);

    //cannot read config
    if (!readConfig(argc, argv))
    {
        return false;
    }

    //username or password is missing
    if ((cfg.username.size() == 0) ||
        (cfg.password.size() != 32))
    {
        return false;
    }

    return true;
}

bool Lfms::readConfig(int argc, char* argv[])
{
    //read command line and display help or version if requested
    if (!cfg.readCommandLine(argc, argv))
    {
        return false;
    }
    else if (cfg.displayHelp)
    {
        printf("Usage: lfms [options]\n\n\
Options:\n\
  -h               print this help\n\
  -v               print version\n\
  -q               be quiet. Do not output anything to stdout\n\
  -c <config_file> use <config_file> as configuration file;\n\
                   default is ~/.config/lfms/config\n\
  -a <mode>        which action to perform, where mode is one of:\n\
                   'n' - update Now Playing information\n\
                   's' - scrobble a track\n\
\n\
  --artist <artist>\n\
                   The artist name\n\
  --track  <track> The track name\n\
  --album  <album> The track name\n\
  --track-number <number>\n\
                   The track number of the track on the album.\n\
  --timestamp <timestamp>\n\
                   The time the track started playing\n\
");
    }
    else if (cfg.displayVersion)
    {
        printf("lfms, version %s\n", LFMS_VERSION);
    }

    //error reading config file
    if (!cfg.readConfigFile())
    {
        fprintf(stderr, "%s\n", cfg.getErrorMessage().c_str());
        //        cfg.save();
        return false;
    }
    //print debug message
    else
    {
        printf("config file '%s' read successfully\n", cfg.configFile.c_str());
    }

    return true;
}

bool Lfms::initSession(bool forceNew)
{
    bool sessionIsNew = false;

    //read existing session
    //only if new session wasn't explicitly requested
    if (!forceNew)
    {
        session.restore(cfg.sessionFile);
    }

    //request new session in case any of following conditions is true
    // - it was specifically asked
    // - existing session is not ok
    if (forceNew || (session.getStatus().compare("ok") != 0))
    {
        session = api.getMobileSession(cfg.username, cfg.password);
        sessionIsNew = true;
    }

    if (sessionIsNew)
    {
        session.save(cfg.sessionFile);
    }

    api.setSessionId(session.getId());

    return (session.getStatus().compare("ok") == 0);
}

bool Lfms::action()
{
    switch (cfg.mode)
    {
        case 's':
            //          retval = submit_track();
            break;
        case 'n':
            return nowPlaying();
            break;
    }

    return true;
}

bool Lfms::fillTrackInfo(LfmsTrack& track, arrStr& info)
{
    arrStr::iterator it;

    it = info.find("artist");

    if (it == info.end())
    {
        return false;
    }

    track.artist = (*it).second;

    it = info.find("track");

    if (it == info.end())
    {
        return false;
    }

    track.track = (*it).second;

    //non-mandatory track info
    it = info.find("album");

    if (it != info.end())
    {
        track.album = (*it).second;
    }

    //timestamp
    it = info.find("timestamp");

    if (it == info.end())
    {
        track.timestamp = time(0);
    }
    else
    {
        track.timestamp = atoi((*it).second.c_str());
    }
    
    return true;
}

bool Lfms::nowPlaying()
{
    LfmsTrack track;

    if (!fillTrackInfo(track, cfg.otherParams))
    {
        return false;
    }

    if (initSession())
    {
        if (!api.updateNowPlaying(track))
        {
            return false;
        }

        return true;
    }

    return false;
}
