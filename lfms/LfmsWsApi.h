#include <string>
#include "main.h"

class LfmsWsApi {
    string apiKey, apiSecret, apiUrl;
    int apiPort;

    string sessionId;

    string signCall(const string&);

 public:
    int setAccountInfo(const string&, const string&);
    int setServiceInfo(const string&, int);
    int setSessionId(const string&);

    string call(const string&, paramsMap);
    string getMobileSession();
}
