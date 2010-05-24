#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

//own headers
//#include "./main.h"

using namespace std;

int main(int argc, char* argv[])
{
  int i, argument;
  string filename, config_file;
  FILE *file;
  char fline[100];

  do
  {
    argument = getopt(argc, argv, "vhf:");

    switch (argument)
    {
      case 'h':
        printf("help screen\n");
        return 0;
      case 'v':
	printf("t1, version 0.0.1\n");
        return 0;
      case 'f':
	filename = optarg;
        break;
    }
  }
  while (argument != -1);

  if (filename.length() > 0)
  {
    file = fopen(filename.c_str(), "r");
    if (NULL == file)
    {
      printf("cannot read file '%s'\n", filename.c_str());
      return 1;
    }
    else
    {
      while (!feof(file))
      {
	fgets(fline, sizeof(fline), file);
	config_file.append(fline);
	printf("read bytes: %03d, buffer size: %04d\n", strlen(fline), config_file.length());
	fline[0] = 0;
      }

      fclose(file);

      cout << config_file.data();
    }
  }

  return 0;
}

int read_config(char *path)
{


}
