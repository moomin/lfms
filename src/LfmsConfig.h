#include <string>
#include "helpers.h"

#ifndef LFMS_CONFIG_H
#define LFMS_CONFIG_H

class LfmsConfig {
    std::string error;

 public:
    //command-line params
    bool displayVersion, displayHelp, quiet, debug;
    std::string configDir;
    std::string dataDir;

    std::string configFile;
    std::string queueFile;
    std::string sessionFile;

    std::string logFile;
    std::string socketLog;
    std::string lockFile;

    //config file params
    std::string username, password;
    char action;

    arrStr otherParams; 

    //methods
    LfmsConfig();
    std::string getErrorMessage();

    bool save();
    bool readConfigFile();
    bool readCommandLine(int argc, char *argv[]);
};

#endif
