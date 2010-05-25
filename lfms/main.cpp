#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

using namespace std;

//own headers
#include "main.h"

//configuration structure
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

  printf("user: %s\n", cfg.username.c_str());
  printf("pass: %s\n", cfg.password.c_str());
  printf("host: %s\n", cfg.host.c_str());
  printf("port: %d\n", cfg.port);

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

int read_session()
{
    return 0;
}

int handshake()
{
    int retval;

    retval = read_session();

    //if read_session failed or session is not active then perform a handshake
    if ((retval != 0) || !session.is_active)
    {
        
    }

    return retval;
}

int submit_track()
{

}

int now_playing()
{

}
