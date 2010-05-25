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

int main(int argc, char* argv[])
{
  short int retval;
  string config_file;

  //initialize defaults;
  config_file = "~/.config/lfms/config";

  retval = parse_command_line(argc, argv, &config_file);
  //help or version was requested
  if ((0 < retval) && (retval <= 2))
  {
    return 0;
  }
  //unexpected error
  else if (retval > 2)
  {
    return 1;
  }

  retval = read_config(&config_file);
  //error reading config file
  if (0 != retval)
  {
    return 1;
  }
  //print debug message
  else
  {
    printf("config file '%s' read successfully\n", config_file.c_str());
  }

  printf("user: %s\n", cfg.username.c_str());
  printf("pass: %s\n", cfg.password.c_str());

  return 0;
}

int parse_command_line(int argc, char *argv[], string *config_file)
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
 -c <config_file> use <config_file> as configuration file; default is ~/.config/lfms/config\n\n");
        return 1;
      case 'v':
	printf("lfms, version 0.0.1\n");
        return 2;
      case 'c':
	*config_file = optarg;
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
  string config;
  const char *param_username = "username";
  const char *param_password = "password";
  string config_line;
  string::iterator it;
  FILE *file;
  char fline[1000];

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
      while (!feof(file) && (config.length() <= max_config_size))
      {
	fgets(fline, sizeof(fline), file);
	config += fline;
	fline[0] = 0;
      }

      if (!feof(file) || (config.length() > max_config_size))
      {
	fprintf(stderr, "config file '%s' is larger than %d bytes; exiting\n", path->c_str(), max_config_size);
	retval = 2;
      }
      else
      {

	int char_pos = 0, current_pos = 0;
	
	while (char_pos < config.length())
	{
	  current_pos = char_pos;
	  char_pos = config.find("\n", current_pos);
	  string line = config.substr(current_pos, char_pos);
	  string param = line.substr(0, line.find("="));

	  if (param.length() < line.length())
	  {
	    string value = line.substr(param.length() + 1);

	    if (param.compare("username") == 0)
	    {
	      cfg.username = value;
	    }
	    else if (param.compare("password") == 0)
	    {
	      cfg.password = value;
	      printf("length: %02d\n", param.length());
	    }

	  }

	  char_pos = current_pos + line.length() + 1;
	}
      }

      fclose(file);
    }

  }

  return retval;
}
