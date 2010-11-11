#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

using namespace std;

//own headers
#include "main.h"
#include "helpers.h"
#include "LfmsConfig.h"

//global structures
LfmsConfig cfg;
lastfm_session session;

int main(int argc, char* argv[])
{
  short int retval;

  session.is_active = false;

  retval = cfg.readCommandLine(argc, argv);

  //help or version was requested or error
  if (cfg.displayVersion || cfg.displayHelp || (retval != 0))
  {
    return retval;
  }

  retval = cfg.readConfigFile();
  //error reading config file
  if (0 != retval)
  {
    fprintf(stderr, "%s\n", cfg.getErrorMessage().c_str());
    return 1;
  }
  //print debug message
  else
  {
    printf("config file '%s' read successfully\n", cfg.configFile.c_str());
  }

  retval = handshake();

  if (retval != 0)
  {
      fprintf(stderr, "error during handshake\n");
  }

  switch (cfg.mode)
  {
      case 's':
          retval = submit_track();
          break;
      case 'n':
          retval = now_playing();
          break;
  }

  return retval;
}

int read_session(string &path)
{
    ifstream file;
    char line[1025];
    short int line_number = 0, retval = 0;
    string lstr;
    
    file.open(resolve_path(path).c_str());

    if(!file.is_open())
    {
      retval = 1;
    }
    else
    {
      while(file.good())
      {
//        file >> noskipws;
//        file >> lstr;

        getline(file, lstr);
        printf("the line: %s\n", lstr.c_str());
        //        file.getline(line, sizeof(line) - 1);

        line_number++;

        //break if newline was not reached
        if (file.fail() && !file.eof())
        {
          retval = 1;
          break;
        }
        else if (line_number == 1)
        {
          session.status = lstr;
          session.is_active = (0 == session.status.compare("OK"));
        }
        else if (line_number == 2)
        {
          session.id = lstr;
        }
        else if (line_number == 3)
        {
          session.now_playing_url = lstr;
        }
        else if (line_number == 4)
        {
          session.submission_url = lstr;

          //we don't need more than 4 lines
          break;
        }
      }

      file.close();
    }

    return retval;
}

int handshake()
{
    int retval;

    retval = read_session(cfg.sessionFile);

    //if read_session failed or session is not active then perform a handshake
    if ((retval != 0) || !session.is_active)
    {
      //perform handshake
      retval = 1;
    }

    printf("\nsession info:\n  is_active: %d\n  status: %s\n  id: %s\n  now_playing: %s\n  submission: %s\n\n",\
           session.is_active,
           session.status.c_str(),
           session.id.c_str(),
           session.now_playing_url.c_str(),
           session.submission_url.c_str());

    return retval;
}

int submit_track()
{

}

int now_playing()
{

}

