#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <map>

typedef std::map<std::string, std::string> arrStr;
typedef std::map<short int, std::string> arrIndex;
//typedef std::pair<std::string, std::string> paramsPair;

std::string resolve_path(const std::string &);
std::string get_md5hex(const std::string &);
bool is_file_exist(const char*);
bool make_dir(const char*, bool recursive = false);

#endif
