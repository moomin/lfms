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

  retval = cfg.readCommandLine(argc, argv);

  api.setAccountInfo("apikeyvalue", "apisecretvalue");
  api.setServiceInfo("http://localhost/", 8001);

  //help or version was requested or error
  if (cfg.displayVersion || cfg.displayHelp || (retval != 0))
  {
      return retval;
  }

  //error reading config file
  if (0 != cfg.readConfigFile())
  {
    fprintf(stderr, "%s\n", cfg.getErrorMessage().c_str());
    return 1;
  }
  //print debug message
  else
  {
    printf("config file '%s' read successfully\n", cfg.configFile.c_str());
  }

  if (session.restore(cfg.sessionFile))
  {
      api.getMobileSession(cfg.username, cfg.password);
      fprintf(stderr, "session error: %s\n", session.getErrorMessage().c_str());
  }

  switch (cfg.mode)
  {
      case 's':
          //          retval = submit_track();
          break;
      case 'n':
          //          retval = now_playing();
          break;
  }

  return retval;
}
