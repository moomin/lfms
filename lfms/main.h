#ifndef MAIN_H
#define MAIN_H

#include <map>
#define LFMS_VERSION "0.1"
#define LFMS_CLIENT_STRING "tst"
#define LFMS_API_KEY "1234567890qwertyuioplkjhgfdsazxc"
#define LFMS_SECRET "mysecret"

int submit_track();
int now_playing();

typedef std::map<std::string, std::string> paramsMap;
typedef std::pair<std::string, std::string> paramsPair;

#endif
