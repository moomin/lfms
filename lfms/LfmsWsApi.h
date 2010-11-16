#include <string>
#include "main.h"
#include "LfmsSession.h"

using namespace std;

class LfmsWsApi {
    string apiKey, apiSecret, apiUrl;
    string sessionId;

    string getCallSignature(paramsMap&);
    string call(const string&, paramsMap&);

 public:
    int setAccountInfo(const string&, const string&);
    int setServiceInfo(const string&);
    int setSessionId(const string&);

    LfmsSession getMobileSession(const string&, const string&);
};
