#include <ctype.h>
#include <cstdlib>
#include <string.h>
#include <cstdio>
#include "HttpClient.h"

//for sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

bool HttpClient::parseUrl(const string & url)
{
    const string hostPrefix = "http://";
    const int prefixLength = hostPrefix.length();

    if (url.npos != url.find(hostPrefix))
    {
        hostname = url.substr(prefixLength,
                              (url.find("/", prefixLength) - prefixLength));
    }

    if (hostname.length())
    {
        size_t portStart = hostname.find(":");

        if (hostname.npos != portStart)
        {
            port = hostname.substr(portStart + 1);
            hostname.resize(portStart);
        }
        else
        {
            port = "80";
        }

        uri = url.substr(url.find("/", prefixLength + hostname.length()));
    }

    return (hostname.length() != 0);
}

bool HttpClient::sendRequest(const string& method, const string& url, paramsMap& params)
{
    paramsMap headers;
    return sendRequest(method, url, params, headers);
}

bool HttpClient::sendRequest(const string& method, const string& url, paramsMap& params, paramsMap& headers)
{
    string request, headersBody, messageBody;
    string paramsStr;
    paramsMap::iterator it;

    if ((method.compare("GET") != 0) && 
        (method.compare("POST") != 0))
    {
        return false;
    }

    if (!parseUrl(url))
    {
        return false;
    }

    //prepare parameters
    for (it = params.begin(); it != params.end(); it++)
    {
        if (it != params.begin())
        {
            paramsStr += "&";
        }

        paramsStr += escapeUrl((*it).first) +"="+ escapeUrl((*it).second);
    }

    //append parameters to URI
    if (method.compare("GET") == 0)
    {
        uri += "?" + paramsStr;
    }
    //POST request requires Content-Length header
    else
    {
        char lengthChar[32];
        sprintf(lengthChar, "%d", paramsStr.length());
        headers["Content-Length"] = lengthChar;
        messageBody = paramsStr;
    }

    headers["Host"] = hostname;
    headers["Connection"] = "Close";

    //prepare headers body
    for (it = headers.begin(); it != headers.end(); it++)
    {
        headersBody += (*it).first +": "+ (*it).second + "\r\n";
    }
    
    //form request line
    request = method + " " + uri + " HTTP/1.0\r\n";
    request += headersBody;
    request += "\r\n" + messageBody;

    printf("request %s\n", request.c_str());

    int sock = open();
    if (sock && send(sock, request))
    {
        return getResponse(sock);
    }
    else
    {
        return false;
    }
}

string HttpClient::escapeUrl(const string& src, bool cgi_value)
{
    const char* as_is_chars;
    string dest;
    if (cgi_value)
        as_is_chars = ".,-_";
    else
        as_is_chars = ".,-_&;=/:?";

    size_t srclen = src.size();
    size_t bufflen = srclen*3 +1;
    char* buff;
    char* buffptr;

    buff = static_cast<char*>(malloc(bufflen));

    /*
    if (!buff)
        throw std::bad_alloc();
    */

    buffptr = buff;

    for (size_t u=0; u < srclen; ++u)
    {
        unsigned char c = src[u];
        if (c == ' ' && cgi_value)
            *(buffptr++) = '+';
        else if (c == 0)
        {
            *(buffptr++) = '%';
            *(buffptr++) = '0';
            *(buffptr++) = '0';
        }
        else
        if (isascii(c)
            && (isalnum(c)
                || strchr(as_is_chars, c) != NULL
                )
            )
        {
            *(buffptr++) = c;
        }
        else
        {
            snprintf(buffptr, 4, "%%%02X", c);
            buffptr += 3;
        }
    }
    *buffptr = 0;

    dest = buff;

    free(buff);

    return dest;
}

int HttpClient::open()
{
    int sock;
    struct addrinfo *addresses, hints, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &addresses) != 0)
    {
        return false;
    }

    for (p = addresses; p != 0; p = p->ai_next)
    {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1)
        {
            continue;
        }

        break;
    }
    
    if (p == 0)
    {
        freeaddrinfo(addresses);
        return 0;
    }

    freeaddrinfo(addresses);

    return sock;
}

bool HttpClient::send(int sock, const string& data)
{
    int sentBytes;

    sentBytes = ::send(sock, data.c_str(), data.length(), 0);
    return (sentBytes == data.length());
}

int HttpClient::getResponse(int sock)
{
    paramsMap headers;
    string response, line;
    char buffer[1024*4 + 1];
    int from = 0, to = 0, n;

    //read socket
    while(n = ::recv(sock, buffer, 1024*4, 0))
    {
        buffer[n] = 0;
        response += buffer;
    }

    responseBody = response;

    ::close(sock);

    //get HTTP response status
    line = response.substr(0, response.find("\r\n"));
    responseStatus = line.substr(9, line.find(" ", 9) - 9);

    //cycle through headers
    while (to != response.npos)
    {
        to = response.find("\r\n", from);

        line = response.substr(from, to - from);

        if (line.length() == 0)
        {
            //get HTTP response body
            responseBody = response.substr(from + 2);
            break;
        }

        from = to + 2;
    }

    return responseStatus.compare("200") == 0;
}

string HttpClient::getResponseBody()
{
    return responseBody;
}

string HttpClient::getResponseStatus()
{
    return responseStatus;
}
