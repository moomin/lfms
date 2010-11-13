#include <fstream>
#include "LfmsSession.h"
#include "helpers.h"

string LfmsSession::getErrorMessage()
{
    return error;
}

int LfmsSession::restore(const string & path)
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
                key = line;
            }
            else if (line_number == 2)
            {
                name = line;
                break;
            }
        }

        file.close();
    }

    return (error.length() > 0);
}
