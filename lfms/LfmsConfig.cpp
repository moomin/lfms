#include <fstream>
#include "LfmsConfig.h"
#include "helpers.h"

using namespace std;

LfmsConfig::LfmsConfig()
{
    //initialize defaults;
    displayVersion = false;
    displayHelp = false;
    configDir   = "~/.config/lfms";
    configFile  = configDir + "/config";
    sessionFile = configDir + "/session";
    queueFile   = configDir + "/queue";
    mode = 's';

    if (!is_file_exist(resolve_path(configDir).c_str()) &&
        !make_dir(resolve_path(configDir).c_str(), true))
    {
        configDir.clear();
    }
}

string LfmsConfig::getErrorMessage()
{
    return error;
}

bool LfmsConfig::save()
{
    ofstream file;
    
    file.open(resolve_path(configFile).c_str(), ios_base::trunc);

    if(!file.is_open())
    {
        error = "cannot create config file";
        return false;
    }

    file << "username=" << endl;
    file << "password=" << endl;
    
    file.close();

    return true;
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
          error = "cannot read config file";
          if (configDir.size() &&
              !is_file_exist(resolve_path(configFile).c_str()))
          {
              error = "empty config file was created";
              save();
          }
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
              }
          }

          file.close();
          return true;
      }
  }
  else
  {
      error = "zero-length path to config file";
  }

  return false;
}

bool LfmsConfig::readCommandLine(int argc, char *argv[])
{
    int argument;
    
    do
    {
        argument = getopt(argc, argv, "vhc:m:");
            
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
        case 'm':
            mode = *optarg;
            break;
        }
    }
    while (argument != -1);

    return true;
}
