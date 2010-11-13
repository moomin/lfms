#include <string>
#include "main.h"

using namespace std;

class LfmsWsApi {
    string apiKey, apiSecret, apiUrl;
    int apiPort;

    string sessionId;

    string getCallSignature(paramsMap&);
    string call(const string&, paramsMap&);

 public:
    int setAccountInfo(const string&, const string&);
    int setServiceInfo(const string&, int);
    int setSessionId(const string&);

    string getMobileSession(const string&, const string&);
};
