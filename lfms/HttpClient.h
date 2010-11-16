#include <string>
#include "main.h"

using namespace std;

class HttpClient {
    string hostname;
    string port;
    string uri;

    string responseStatus, responseBody;

    int open();
    bool send(int, const string&);
    bool parseUrl(const string&);
    string escapeUrl(const string& src, bool cgi_value = false);
    int getResponse(int);

 public:

    bool sendRequest(const string&, const string&, paramsMap&);
    bool sendRequest(const string&, const string&, paramsMap&, paramsMap&);
    string getResponseBody();
    string getResponseStatus();
};
