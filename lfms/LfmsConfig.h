#include <string>

#ifndef LFMS_CONFIG_H
#define LFMS_CONFIG_H

#define MAX_CONFIG_SIZE 8192

using namespace std;

class LfmsConfig {
    string error;

 public:
    //command-line params
    bool displayVersion, displayHelp;
    string configFile, queueFile, sessionFile;

    //config file params
    string username, password, host;
    int port;
    char mode;

    //methods
    LfmsConfig();
    void print();
    bool save(string name);
    string getErrorMessage();

    bool readConfigFile();
    bool readCommandLine(int argc, char *argv[]);
};

#endif
