#include <cstdio>
#include <queue>
#include "LfmsWsApi.h"
#include "HttpClient.h"
#include "helpers.h"

int LfmsWsApi::getErrorCode()
{
    //dummy
    return 0;
}

string LfmsWsApi::getErrorMessage()
{
    //dummy
    return "dummy error message";
}

bool LfmsWsApi::setAccountInfo(const string& key, const string& secret)
{
    apiKey = key;
    apiSecret = secret;

    return true;
}

bool LfmsWsApi::setServiceInfo(const string& url)
{
    apiUrl = url;

    return true;
}

bool LfmsWsApi::setSessionId(const string& id)
{
    sessionId = id;

    return true;
}

string LfmsWsApi::getCallSignature(arrStr& params)
{
    arrStr::iterator it;
    string stringToSign;

    //order all the parameters alphabetically by parameter
    //name and concatenate them into one string using
    //a <name><value> scheme
    for (it = params.begin(); it != params.end();)
    {
        if ((*it).second.length())
        {
            stringToSign += (*it).first + (*it).second;
            it++;
        }
        else
        {
            params.erase(it++);
        }
    }

    //append secret to this string
    stringToSign += apiSecret;

    printf("string to sign: %s\n", stringToSign.c_str());
    return get_md5hex(stringToSign);
}

bool LfmsWsApi::call(const string& method, arrStr& params, bool isWrite)
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
        response.init(http.getResponseBody().c_str());
        return (response.xpath("/lfm/@status").compare("ok") == 0) ? true : false;
    }
    else
    {
        printf("http error: %s\nhttp body: %s\n",
               http.getResponseStatus().c_str(),
               http.getResponseBody().c_str());

        response.init(http.getResponseBody().c_str());
        return false;
    }
}

LfmsSession LfmsWsApi::getMobileSession(const string& username, const string& password)
{
    arrStr params;

    params["username"] = username;
    //generate token; password must already be an md5 string
    params["authToken"] = get_md5hex(username + password);

    call("auth.getMobileSession", params);

    LfmsSession session;

    session.set(response.xpath("/lfm/@status"),
		response.xpath("/lfm/session/name"),
		response.xpath("/lfm/session/key"),
		(response.xpath("/lfm/session/subscriber").compare("0") == 0) ? true : false);

    return session;
}

bool LfmsWsApi::updateNowPlaying(LfmsTrack& track)
{
    arrStr params;
    string response;
    char itoaHolder[64];

    params["track"] = track.track;
    params["artist"] = track.artist;
    params["album"] = track.album;
    params["albumArtist"] = track.albumArtist;

    if (track.trackNumber)
    {
        sprintf(itoaHolder, "%d", (int)track.trackNumber);
        params["trackNumber"] = itoaHolder;
    }

    params["mbid"] = track.album;

    if (track.duration)
    {
        sprintf(itoaHolder, "%d", (int)track.duration);
        params["duration"] = itoaHolder;
    }

    params["sk"] = sessionId;

    return call("track.updateNowPlaying", params, true);
}

bool LfmsWsApi::scrobble(const LfmsTrack& track)
{
    arrStr params;
    string response;
    char itoaHolder[64];

    params["track"] = track.track;
    params["artist"] = track.artist;
    params["album"] = track.album;
    params["albumArtist"] = track.albumArtist;
    params["streamId"] = track.streamId;

    if (track.trackNumber)
    {
        sprintf(itoaHolder, "%d", (int)track.trackNumber);
        params["trackNumber"] = itoaHolder;
    }

    params["mbid"] = track.album;

    if (track.duration)
    {
        sprintf(itoaHolder, "%d", (int)track.duration);
        params["duration"] = itoaHolder;
    }

    if (track.timestamp)
    {
        sprintf(itoaHolder, "%u", (int)track.timestamp);
        params["timestamp"] = itoaHolder;
    }

    params["sk"] = sessionId;

    return call("track.scrobble", params, true);
}

bool LfmsWsApi::scrobble(queue<LfmsTrack>& tracks)
{
    return false;
}
