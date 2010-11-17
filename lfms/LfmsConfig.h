#include <string>

#ifndef LFMS_CONFIG_H
#define LFMS_CONFIG_H

class LfmsConfig {
    std::string error;

 public:
    //command-line params
    bool displayVersion, displayHelp;
    std::string configFile, queueFile, sessionFile;

    //config file params
    std::string username, password;
    char mode;

    //methods
    LfmsConfig();
    std::string getErrorMessage();

    bool save();
    bool readConfigFile();
    bool readCommandLine(int argc, char *argv[]);
};

#endif
