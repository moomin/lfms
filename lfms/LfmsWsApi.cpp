#include <cstdio>
#include "LfmsWsApi.h"
#include "HttpClient.h"
#include "helpers.h"

int LfmsWsApi::setAccountInfo(const string& key, const string& secret)
{
    apiKey = key;
    apiSecret = secret;

    return 0;
}

int LfmsWsApi::setServiceInfo(const string& url)
{
    apiUrl = url;

    return 0;
}

int LfmsWsApi::setSessionId(const string& id)
{
    sessionId = id;

    return 0;
}

string LfmsWsApi::getCallSignature(paramsMap& params)
{
    paramsMap::iterator it;
    string stringToSign;

    //order all the parameters alphabetically by parameter
    //name and concatenate them into one string using
    //a <name><value> scheme
    for (it = params.begin(); it != params.end(); it++)
    {
        stringToSign += (*it).first + (*it).second;
    }

    //append secret to this string
    stringToSign += apiSecret;

    return get_md5hex(stringToSign);
}

string LfmsWsApi::call(const string& method, paramsMap& params, bool isWrite)
{
    //add parameters required in all calls
    params["api_key"] = apiKey;
    params["method"] = method;

    //add signature
    params["api_sig"] = getCallSignature(params);

    HttpClient http;

    if (http.sendRequest(isWrite ? "POST" : "GET", apiUrl, params))
    {
        printf("http answer: %s\n", http.getResponseBody().c_str());
    }
    else
    {
        printf("http error: %s\nhttp body: %s\n",
               http.getResponseStatus().c_str(),
               http.getResponseBody().c_str());
    }

    return http.getResponseBody();
}

LfmsSession LfmsWsApi::getMobileSession(const string& username, const string& password)
{
    paramsMap params;

    params["username"] = username;
    //generate token; password should already be an md5 string
    params["authToken"] = get_md5hex(username + password);

    call("auth.getMobileSession", params);

    LfmsSession session;

    return session;
}