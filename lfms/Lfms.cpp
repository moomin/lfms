#include <cstdio>
#include <cstdlib>
#include <time.h>

#include "Lfms.h"
#include "LfmsTrack.h"
#include "helpers.h"
#include "Log.h"

bool Lfms::init(int argc, char* argv[])
{
    api.setAccountInfo(LFMS_API_KEY, LFMS_API_SECRET);
    api.setServiceInfo(LFMS_API_URL);
 
    //initalize logs (console and file)
    Log log;

    log.init(cfg.logFile, 'a');
    log.log(LOG_INFO, "starting");

    //cannot read config
    if (!readConfig(argc, argv))
    {
        log.log(LOG_ERR, "cannot read config file: ~/.config/lfms/config\n");
        return false;
    }

    //username or password is missing
    if ((cfg.username.size() == 0) ||
        (cfg.password.size() != 32))
    {
        fprintf(stderr, "username/password empty. Please add it to ~/.config/lfms/config\n");
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
    switch (cfg.action)
    {
        case 's':
            scrobble();
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

    it = info.find("track");
    if (it != info.end())
    {
        track.track = (*it).second;
    }

    it = info.find("artist");
    if (it != info.end())
    {
        track.artist = (*it).second;
    }

    //non-mandatory track info
    it = info.find("album");
    if (it != info.end())
    {
        track.album = (*it).second;
    }

    it = info.find("album-artist");
    if (it != info.end())
    {
        track.albumArtist = (*it).second;
    }

    it = info.find("streamid");
    if (it != info.end())
    {
        track.streamId = (*it).second;
    }

    it = info.find("track-number");
    if (it != info.end())
    {
        track.trackNumber = atoi((*it).second.c_str());
    }

    it = info.find("mbid");
    if (it != info.end())
    {
        track.mbid = (*it).second;
    }

    it = info.find("duration");
    if (it != info.end())
    {
        track.duration = atoi((*it).second.c_str());
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
    
    if (!track.track.length() ||
	!track.artist.length())
    {
        return false;
    }

    return true;
}

bool Lfms::nowPlaying()
{
    LfmsTrack track;

    if (!fillTrackInfo(track, cfg.otherParams))
    {
        //here we should display some message
        fprintf(stderr, "something went wrong\n");
        return false;
    }

    if (initSession())
    {
        if (!api.updateNowPlaying(track))
        {
	    //here we should display some message
            return false;
        }

        return true;
    }

    return false;
}

bool Lfms::scrobble()
{
    LfmsTrack track;

    if (!fillTrackInfo(track, cfg.otherParams))
    {
        //here we should display some message
        fprintf(stderr, "something went wrong\n");
        return false;
    }

    if (initSession())
    {
        if (!api.scrobble(track))
        {
	    //here we should display some message
            return false;
        }

        return true;
    }

    return false;
}
