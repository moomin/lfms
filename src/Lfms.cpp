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
 
    //initalize logs (console and file)
    log.init(cfg.logFile, 'a');
    socketLog.init(cfg.socketLog, 'a');

    //cannot read config
    if (!readConfig(argc, argv))
    {
        log.log(LOG_ERR, "%s", cfg.getErrorMessage().c_str());
        return false;
    }

    //tune logging according to config
    log.console = cfg.quiet ? false : true;
    log.level = cfg.debug ? LOG_DEBUG : LOG_INFO;
    socketLog.level = cfg.debug ? LOG_DEBUG : LOG_INFO;

    log.log(LOG_DEBUG, "config file '%s' read successfully", cfg.configFile.c_str());

    //username or password is missing
    if ((cfg.username.size() == 0) ||
        (cfg.password.size() != 32))
    {
        log.log(LOG_ERR, "username/password empty. Please add it to ~/.config/lfms/config");
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
    else if (cfg.displayHelp || cfg.action == 0)
    {
        printf("Usage: lfms [options]\n\n\
Options:\n\
  -h               print this help\n\
  -v               print version\n\
  -q               be quiet. Do not output anything to stdout\n\
  -d               enable debug output\n\
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
        return false;
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
        log.log(LOG_DEBUG, "Saving session info");
        session.save(cfg.sessionFile);
    }

    api.setSessionId(session.getId());

    if (session.getStatus().compare("ok") != 0)
    {
        log.log(LOG_ERR, "Error initializing session");
        return false;
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
        log.log(LOG_ERR, "Empty Track/Artist name");
        return false;
    }

    return true;
}

bool Lfms::action()
{
    switch (cfg.action)
    {
        case 's':
          return scrobble();
          break;
        case 'n':
          return nowPlaying();
          break;
        default:
          return false;
    }

    return false;;
}

bool Lfms::nowPlaying(short int tryNumber)
{
    LfmsTrack track;
    const char* operation = "Now playing";

    if (!fillTrackInfo(track, cfg.otherParams) || !initSession())
    {
        return false;
    }

    if (api.updateNowPlaying(track))
    {
        logApiSuccess(track, operation);
        return true;
    }

    logApiError(track, operation);

    int errorCode = api.getErrorCode();

    if ((errorCode == 9) && (tryNumber < 3))
    {
        log.log(LOG_INFO, "Retrying, attempt %d", tryNumber);
        return nowPlaying(tryNumber + 1);
    }

    return false;
}

bool Lfms::scrobble(short int tryNumber)
{
    LfmsTrack track;
    const char* operation = "Scrobbling";

    if (!fillTrackInfo(track, cfg.otherParams) || !initSession())
    {
        return false;
    }

    if (api.scrobble(track))
    {
        logApiSuccess(track, operation);
        return true;
    }

    logApiError(track, operation);

    int errorCode = api.getErrorCode();

    if ((errorCode == 9) && (tryNumber < LFMS_MAX_SCROBBLE_TRIES))
    {
        log.log(LOG_INFO, "Retrying, attempt %d", tryNumber);
        return scrobble(tryNumber + 1);
    }
    //TODO: put an error code descripiton here
    else if (errorCode == 9)
    {
        return false;
    }
    //TODO: put an error code descripiton here
    else if ((errorCode == 11) || (errorCode == 16))
    {
        //queue the request
        putTrackIntoQueue(track);
        return false;
    }
    //TODO: add a comment about possible cause
    else
    {
        //just fail
        return false;
    }
}

bool Lfms::submitQueue()
{
    return true;
}

void Lfms::logApiError(const LfmsTrack &track, const char* operation)
{
    log.log(LOG_ERR, "%s failed. Artist: \"%s\", Track: \"%s\". %s",
            operation,
            track.artist.c_str(),
            track.track.c_str(),
            api.getLastCallInfo().c_str());

    socketLog.log(LOG_ERR, "sent:\n%s", api.getRequest().c_str());
    socketLog.log(LOG_ERR, "rcvd:\n%s", api.getResponse().c_str());

    return;
}

void Lfms::logApiSuccess(const LfmsTrack &track, const char* operation)
{
    log.log(LOG_INFO, "%s successfull. Artist: \"%s\", Track: \"%s\"",
            operation,
            track.artist.c_str(),
            track.track.c_str());

    socketLog.log(LOG_DEBUG, "sent:\n%s", api.getRequest().c_str());
    socketLog.log(LOG_DEBUG, "rcvd:\n%s", api.getResponse().c_str());

    return;
}
