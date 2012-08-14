#include <string>
#include <queue>
#include "XmlParser.h"
#include "LfmsSession.h"
#include "LfmsTrack.h"
#include "Curl.h"
#include "helpers.h"

using namespace std;

class LfmsWsApi {
    string apiKey, apiSecret, apiUrl;
    XmlParser response;
    string sessionId;
    int errorCode;
    string lastCallInfo;
    Curl curl;

    string getCallSignature(arrStr&);
    bool call(const string&, arrStr&, bool = false);
    void setLastCallInfo();
    void setLastCallInfo(const string&);

 public:
    bool setAccountInfo(const string&, const string&);
    bool setServiceInfo(const string&);
    bool setSessionId(const string&);
    int getErrorCode();
    string getLastCallInfo();
    string getRequest();
    string getResponse();

    LfmsSession getMobileSession(const string&, const string&);
    bool updateNowPlaying(LfmsTrack&);
    bool scrobble(const LfmsTrack&);
    bool scrobble(queue<LfmsTrack>&);
};
