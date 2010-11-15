#include <string>
#include "main.h"

using namespace std;

class HttpClient {
    string hostname;
    int port;
    string uri;

    string result;

    int send(const string&);
    bool parseUrl(const string&);
    string escapeUrl(const string& src, bool cgi_value = false);

 public:

    int request(const string&, const string&, paramsMap&);
    int request(const string&, const string&, paramsMap&, paramsMap&);
    string getAnswer();
};
