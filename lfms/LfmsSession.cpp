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

bool LfmsSession::restore(const string & path)
{
    ifstream file;
    short int line_number = 0, retval = 0;
    string line;
    
    file.open(resolve_path(path).c_str());

    if(!file.is_open())
    {
        error = "cannot open session file";
    }
    else
    {
        getline(file, status);
        getline(file, name);
        getline(file, key);
        getline(file, line);
        isSubscriber = (line.compare("0") != 0);

        file.close();
    }

    return (error.length() == 0);
}
