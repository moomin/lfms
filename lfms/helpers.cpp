#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <pwd.h>
#include <limits.h>
#include <cstdio>
#include "helpers.h"

using namespace std;

string resolve_path(const string &path)
{
    struct passwd *pwent;
    struct stat file_stat;
    char buf[PATH_MAX*3];
    string real_path;

    //resolve homedir
    if (path.find("~") == 0)
    {
        pwent = getpwuid(getuid());
        real_path = pwent->pw_dir;
        real_path.append(path.substr(1));
    }

    realpath(real_path.c_str(), buf);
    real_path = buf;

    if ((0 != stat(real_path.c_str(), &file_stat)) || !(file_stat.st_mode & S_IFREG))
    {
        real_path.clear();
    }

    return real_path;
}

string get_md5hex(const string & str)
{
    unsigned char md5[MD5_DIGEST_LENGTH + 1];
    
    //we need this because MD5() expects unsigned char
    vector<unsigned char> charV(str.begin(), str.end());

    char hexCharacter[3];
    int position;
    string hexString;

    MD5(&charV[0], charV.size(), md5);

    for (position = 0; position < MD5_DIGEST_LENGTH; position++)
    {
        sprintf(hexCharacter, "%02x", md5[position]);
        hexString += hexCharacter;
    }

    return hexString;
}
