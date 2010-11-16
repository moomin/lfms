#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>

using namespace std;

//own headers
#include "main.h"
#include "helpers.h"
#include "LfmsConfig.h"
#include "LfmsSession.h"
#include "LfmsWsApi.h"

int main(int argc, char* argv[])
{
  LfmsConfig cfg;
  LfmsSession session;
  LfmsWsApi api;
  short int retval;

  //read command line and display help or version if requested
  if (!cfg.readCommandLine(argc, argv))
  {
      return 1;
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

  api.setAccountInfo(LFMS_API_KEY, LFMS_API_SECRET);
  api.setServiceInfo(LFMS_API_URL);

  if (!session.restore(cfg.sessionFile))
  {
      session = api.getMobileSession(cfg.username, cfg.password);
  }

  if (session.getStatus().compare("ok") == 0)
  {
      fprintf(stderr, "session status is: %s; cannot proceed\n", session.getStatus().c_str());
  }

  api.setSessionId(session.getId());

  switch (cfg.mode)
  {
      case 's':
          //          retval = submit_track();
          break;
      case 'n':
          //          retval = now_playing();
          break;
  }

  return 0;
}
