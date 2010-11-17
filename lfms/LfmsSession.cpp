#include <fstream>
#include "LfmsSession.h"
#include "helpers.h"

LfmsSession::LfmsSession()
{
    status = "";
    name = "";
    key = "";
    isSubscriber = false;
}

string LfmsSession::getErrorMessage()
{
    return error;
}

string LfmsSession::getStatus()
{
    return status;
}

string LfmsSession::getId()
{
    return key;
}

bool LfmsSession::restore(const string& path)
{
    ifstream file;
    short int line_number = 0, retval = 0;
    string line;
    
    file.open(resolve_path(path).c_str());

    if(!file.is_open())
    {
        error = "cannot open session file";
        return false;
    }

    getline(file, status);
    getline(file, name);
    getline(file, key);
    getline(file, line);
    isSubscriber = (line.compare("0") != 0);

    file.close();

    return (error.length() == 0);
}

bool LfmsSession::save(const string& path)
{
    ofstream file;
    
    file.open(resolve_path(path).c_str(), ios_base::trunc);

    if(!file.is_open())
    {
        error = "cannot open session file";
        return false;
    }

    file << status << endl;
    file << name << endl;
    file << key << endl;
    file << isSubscriber << endl;
    
    file.close();

    return true;
}

bool LfmsSession::set(const string& status, const string& name, const string& key, bool isSubscriber)
{
    LfmsSession::status = status;
    LfmsSession::name = name;
    LfmsSession::key = key;
    LfmsSession::isSubscriber = isSubscriber;

    return true;
}
