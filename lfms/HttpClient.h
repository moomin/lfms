#include <string>
#include "helpers.h"

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

    bool sendRequest(const string&, const string&, arrStr&);
    bool sendRequest(const string&, const string&, arrStr&, arrStr&);
    string getResponseBody();
    string getResponseStatus();
};
