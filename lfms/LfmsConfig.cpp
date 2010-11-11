#include "LfmsConfig.h"
#include "helpers.h"

LfmsConfig::LfmsConfig()
{
    //initialize defaults;
    displayVersion = false;
    displayHelp = false;
    configFile =  "~/.config/lfms/config";
    queueFile = "~/.config/lfms/queue";
    sessionFile = "~/.config/lfms/session";
    mode = 's';
    host = "localhost";
    port = 8008;
}

void LfmsConfig::print()
{

}

int LfmsConfig::save(string name)
{
    return 0;
}

string LfmsConfig::getErrorMessage()
{
    return error;
}

int LfmsConfig::readConfigFile()
{
  short int retval = 0;
  string line_buffer;
  FILE *file;
  char read_buffer[200];

  if (configFile.length() > 0)
  {
    file = fopen(resolve_path(configFile).c_str(), "r");

    if (NULL == file)
    {
        error = "cannot read config file ''";
        retval = 1;
    }
    else
    {
      //read file
      while (!feof(file) && (ftell(file) <= MAX_CONFIG_SIZE))
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
                  username = value;
                }
                else if (param.compare("password") == 0)
                {
                  password = value;
                }
                else if (param.compare("host") == 0)
                {
                  host = value;
                }
                else if (param.compare("port") == 0)
                {
                  port = atoi(value.c_str());
                }

              }

              //clear line_buffer
              line_buffer.clear();
          }
      }

      //print error if file is too big
      if (!feof(file) || (ftell(file) > MAX_CONFIG_SIZE))
      {
          error = "config file '%s' is larger than %d bytes";
      }

      fclose(file);
    }

  }
  else
  {
      error = "zero-length path to config file";
  }

  return (error.length() > 1);
}

int LfmsConfig::readCommandLine(int argc, char *argv[])
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
            displayHelp = true;
            break;
        case 'v':
            printf("lfms, version 0.0.1\n");
            displayVersion = true;
            break;
        case 'c':
            configFile = optarg;
            break;
        }
    }
    while (argument != -1);

    return 0;
}
