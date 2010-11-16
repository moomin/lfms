#include <string>
#include "main.h"

using namespace std;

class HttpClient {
    string hostname;
    string port;
    string uri;

    string resultStatus, resultBody;

    bool send(const string&);
    bool parseUrl(const string&);
    string escapeUrl(const string& src, bool cgi_value = false);

 public:

    string request(const string&, const string&, paramsMap&);
    string request(const string&, const string&, paramsMap&, paramsMap&);
    string getAnswer();
};
