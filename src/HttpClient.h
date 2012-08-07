#include <string>
#include "helpers.h"

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

using namespace std;

class HttpClient {
    string hostname;
    string port;
    string uri;

    int bodyStartPosition;
    string request, response;
    string statusCode;

    int open();
    bool send(int, const string&);
    bool parseUrl(const string&);
    string escapeUrl(const string& src, bool cgi_value = false);
    int setResponse(int);

 public:

    bool sendRequest(const string&, const string&, arrStr&);
    bool sendRequest(const string&, const string&, arrStr&, arrStr&);
    string getRequest();
    string getResponse();
    string getResponseBody();
    string getStatusCode();
};

#endif
