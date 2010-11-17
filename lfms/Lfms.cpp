#include <cstdio>

#include "Lfms.h"
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
        printf("lfms, usage: lfms [-h] [-v] [-c config_file]\n\
   -h               print this help\n\
   -v               print version\n\
   -c <config_file> use <config_file> as configuration file; default is ~/.config/lfms/config\n");
    }
    else if (cfg.displayVersion)
    {
        printf("lfms, version %s\n", LFMS_VERSION);
    }

    //error reading config file
    if (!cfg.readConfigFile())
    {
        fprintf(stderr, "%s\n", cfg.getErrorMessage().c_str());
        return 1;
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
        fprintf(stderr, "session status is: %s; trying to get session key\n", session.getStatus().c_str());
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
            //          retval = now_playing();
            break;
    }

    return true;
}
