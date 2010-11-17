#include "Lfms.h"

int main(int argc, char* argv[])
{
    Lfms lfms;

    if (lfms.init(argc, argv))
    {
        return (lfms.action() ? 0 : 1);
    }

    return 1;
}
