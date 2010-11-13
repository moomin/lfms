#include <fstream>
#include "LfmsSession.h"
#include "LfmsConfig.h"
#include "helpers.h"

LfmsSession::LfmsSession()
{
    isActive = false;
}

int LfmsSession::init(LfmsConfig cfg)
{
    config = cfg;
    return 0;
}

string LfmsSession::getErrorMessage()
{
    return error;
}

int LfmsSession::restore()
{
    ifstream file;
    short int line_number = 0, retval = 0;
    string line;
    
    file.open(resolve_path(config.sessionFile).c_str());

    if(!file.is_open())
    {
        error = "cannot open session file";
    }
    else
    {
        while(getline(file, line))
        {
            printf("the line: %s\n", line.c_str());

            line_number++;

            //break if newline was not reached
            if (file.fail() && !file.eof())
            {
                error = "no newline was detected";
                break;
            }
            else if (line_number == 1)
            {
                status = line;
                isActive = (0 == status.compare("OK"));
            }
            else if (line_number == 2)
            {
                id = line;
                break;
            }
        }

        file.close();
    }

    return (error.length() > 0);
}

int LfmsSession::getMobileSession()
{

    return 0;
}
