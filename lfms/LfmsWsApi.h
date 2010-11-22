#include <string>
#include "LfmsSession.h"
#include "LfmsTrack.h"
#include "helpers.h"

using namespace std;

class LfmsWsApi {
    string apiKey, apiSecret, apiUrl;
    string sessionId;

    string getCallSignature(arrStr&);
    string call(const string&, arrStr&, bool = false);

 public:
    int setAccountInfo(const string&, const string&);
    int setServiceInfo(const string&);
    int setSessionId(const string&);

    LfmsSession getMobileSession(const string&, const string&);
    bool updateNowPlaying(LfmsTrack&);
};
