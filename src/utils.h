#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#define CHDIR _chdir
const char DELIMITER = ';';
#else
#include <unistd.h>
#define CHDIR chdir
const char DELIMITER = ':';
#endif

std::vector<std::string> split(std::string input, char divider = DELIMITER);
std::vector<std::string> split_input(const std::string &str);
bool find_executable_path(const std::string exec_path);

#endif
