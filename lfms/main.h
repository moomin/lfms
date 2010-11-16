#ifndef MAIN_H
#define MAIN_H

#include <map>
#define LFMS_VERSION "0.1"
// #define LFMS_API_URL "http://ws.audioscrobbler.com/2.0/"
#define LFMS_API_URL "http://localhost/~dusoft/ws.php"

#define LFMS_API_KEY "98c7d501c9c52f770dc25a63df6ce7ec"
#define LFMS_API_SECRET "35431b8abe8861a454ed34286033c148"

typedef std::map<std::string, std::string> paramsMap;
typedef std::pair<std::string, std::string> paramsPair;

#endif
