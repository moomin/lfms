#include <string>
#include <sys/stat.h>
#include <pwd.h>
#include "helpers.h"

string resolve_path(string &path)
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

    printf("debug: resolving path '%s' into '%s'\n", path.c_str(), real_path.c_str());
    return real_path;
}
