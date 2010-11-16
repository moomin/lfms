#include <fstream>
#include <cstdlib>
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

bool LfmsConfig::save(string name)
{
    return false;
}

string LfmsConfig::getErrorMessage()
{
    return error;
}

bool LfmsConfig::readConfigFile()
{
  string line;
  ifstream file;

  if (configFile.length() > 0)
  {
      file.open(resolve_path(configFile).c_str());

      if (!file.is_open())
      {
          error = "cannot read config file ''";
      }
      else
      {
          //read file
          while (getline(file, line))
          {
              string param = line.substr(0, line.find("="));

              //read value and update cfg
              if (param.length() < line.length())
              {
                  //treat everything after '=' as value
                  string value = line.substr(param.length() + 1,
                                         line.find("\n") - param.length() - 1);

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
          }

          file.close();
      }

  }
  else
  {
      error = "zero-length path to config file";
  }

  return (error.length() == 0);
}

bool LfmsConfig::readCommandLine(int argc, char *argv[])
{
    int argument;
    
    do
    {
        argument = getopt(argc, argv, "vhc:");
            
        switch (argument)
        {
        case 'h':
            displayHelp = true;
            break;
        case 'v':
            displayVersion = true;
            break;
        case 'c':
            configFile = optarg;
            break;
        }
    }
    while (argument != -1);

    return true;
}
