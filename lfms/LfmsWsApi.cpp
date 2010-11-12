#include "LfmsWsApi.h"

int LfmsWsApi::setAccountInfo(const string& key, const string& secret)
{
    apiKey = key;
    apiSecret = secret;

    return 0;
}

int LfmsWsApi::setServiceInfo(const string& url, const int& port)
{
    apiUrl = url;
    apiPort = port;

    return 0;
}

int LfmsWsApi::setSessionId(const string& id)
{
    sessionId = id;

    return 0;
}

string LfmsWsApi::signCall(const string& params)
{

}

string LfmsWsApi::call(const string& method, paramsMap params)
{
    paramsMap::iterator it;
    
    params.insert(make_pair("api_key", apiKey));

    it = params.begin();

    for ( it = params.begin(); it != params.end(); it++)
    {
        
    }


}
