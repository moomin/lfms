#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

using namespace std;

//own headers
#include "main.h"

//global structures
lfmscfg cfg;
lastfm_session session;

int main(int argc, char* argv[])
{
  short int retval;

  //initialize defaults;
  cfg.display_version = false;
  cfg.display_help = false;
  cfg.config_file =  "~/.config/lfms/config";
  cfg.queue_file = "~/.config/lfms/queue";
  cfg.session_file = "~/.config/lfms/session";
  cfg.mode = 's';
  cfg.host = "localhost";
  cfg.port = 8008;
  session.is_active = false;

  retval = parse_command_line(argc, argv);

  //help or version was requested or error
  if (cfg.display_version || cfg.display_help || (retval != 0))
  {
    return retval;
  }

  retval = read_config(&cfg.config_file);
  //error reading config file
  if (0 != retval)
  {
    return 1;
  }
  //print debug message
  else
  {
    printf("config file '%s' read successfully\n", cfg.config_file.c_str());
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

int parse_command_line(int argc, char *argv[])
{
  int argument;

  do
  {
    argument = getopt(argc, argv, "vhc:");

    switch (argument)
    {
      case 'h':
          printf("lfms, usage: lfms [-h] [-v] [-c config_file]\n\
 -h               print this help\n\
 -v               print version\n\
 -c <config_file> use <config_file> as configuration file; default is ~/.config/lfms/config\n");
          cfg.display_help = true;
          break;
      case 'v':
          printf("lfms, version 0.0.1\n");
          cfg.display_version = true;
          break;
      case 'c':
          cfg.config_file = optarg;
          break;
    }
  }
  while (argument != -1);

  return 0;
}

int read_config(string *path)
{
  const int max_config_size = 8 * 1024;
  short int retval = 0;
  string line_buffer;
  FILE *file;
  char read_buffer[200];

  if (path->length() > 0)
  {
    file = fopen(path->c_str(), "r");

    if (NULL == file)
    {
      fprintf(stderr, "cannot read config file '%s'\n", path->c_str());
      retval = 1;
    }
    else
    {
      //read file
      while (!feof(file) && (ftell(file) <= max_config_size))
      {
          //read a buffer and append it to the line_buffer
          fgets(read_buffer, sizeof(read_buffer), file);
          line_buffer += read_buffer;

          //if newline was found or the end of the file has been reached
          if ((line_buffer.find("\n") != line_buffer.npos) || feof(file))
          {
              string param = line_buffer.substr(0, line_buffer.find("="));
              
              //read value and update cfg
              if (param.length() < line_buffer.length())
              {
                //treat everything after '=' as value
                string value = line_buffer.substr(param.length() + 1, line_buffer.find("\n") - param.length() - 1);

                //update known cfg parameters with values from file
                if (param.compare("username") == 0)
                {
                  cfg.username = value;
                }
                else if (param.compare("password") == 0)
                {
                  cfg.password = value;
                }
                else if (param.compare("host") == 0)
                {
                  cfg.host = value;
                }
                else if (param.compare("port") == 0)
                {
                  cfg.port = atoi(value.c_str());
                }

              }
              
              //clear line_buffer
              line_buffer.clear();
          }
      }

      //print error if file is too big
      if (!feof(file) || (ftell(file) > max_config_size))
      {
          fprintf(stderr, "config file '%s' is larger than %d bytes; exiting\n", path->c_str(), max_config_size);
          retval = 2;
      }

      fclose(file);
    }

  }
  else
  {
      retval = 3;
  }

  return retval;
}

string resolve_path(string &path)
{
    struct passwd *pwent;
    struct stat file_stat;
    char buf[PATH_MAX*3];
    string real_path;

    //resolve homedir
    if (path.find("~") == 0)
    {
        pwent = getpwuid(getuid());
        real_path = pwent->pw_dir;
        real_path.append(path.substr(1));
    }

    realpath(real_path.c_str(), buf);
    real_path = buf;

    if ((0 != stat(real_path.c_str(), &file_stat)) || !(file_stat.st_mode & S_IFREG))
    {
        real_path.clear();
    }

    printf("debug: resolving path '%s' into '%s'\n", path.c_str(), real_path.c_str());
    return real_path;
}

int read_session(string &path)
{
    ifstream file;
    char line[1025];
    short int line_number = 0, retval = 0;
    
    file.open(resolve_path(path).c_str());

    if(!file.is_open())
    {
      retval = 1;
    }
    else
    {
      while(!file.eof())
      {
        file.getline(line, sizeof(line) - 1);

        line_number++;

        //break if newline was not reached
        if (file.fail() && !file.eof())
        {
          retval = 1;
          break;
        }
        else if (line_number == 1)
        {
          session.status = line;
          session.is_active = (0 == session.status.compare("OK"));
        }
        else if (line_number == 2)
        {
          session.id = line;
        }
        else if (line_number == 3)
        {
          session.now_playing_url = line;
        }
        else if (line_number == 4)
        {
          session.submission_url = line;

          //we don't need more than 4 lines
          file.close();
          break;
        }
      }
    }

    return retval;
}

int handshake()
{
    int retval;

    retval = read_session(cfg.session_file);

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

