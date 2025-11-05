#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

#ifdef _WIN32
#include <direct.h>
#define CHDIR _chdir
const char DELIMITER = ';';
#else
#include <unistd.h>
#define CHDIR chdir
const char DELIMITER = ':';
#endif

std::vector<std::string> split(std::string input, char divider = DELIMITER) {
  std::vector<std::string> chunks;
  std::string chunk;
  std::stringstream ss(input);

  while (std::getline(ss, chunk, divider)) {
    chunks.emplace_back(chunk);
  }

  return chunks;
}

bool find_executable_path(const std::string exec_path) {
  if (fs::exists(exec_path)) {
    fs::file_status exec_status = fs::status(exec_path);
    fs::perms exec_perm = exec_status.permissions();
    if ((exec_perm & fs::perms::owner_exec) != fs::perms::none) {
      return true;
    }
  }
  return false;
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // get the path environment variable
  char *process_env = getenv("PATH");
  std::vector<std::string> paths = split(process_env);

  std::vector<std::string> builtins{"type", "exit", "echo", "pwd"};

  std::cout << "$ ";

  std::string command;
  std::getline(std::cin, command);
  if (command == "exit 0") {
    return 0;
  }

  if (command.starts_with("echo")) {
    std::cout << command.substr(5) << std::endl;
  } else if (command.starts_with("type")) {
    std::string arg = command.substr(5);
    if (std::find(builtins.begin(), builtins.end(), arg) != builtins.end()) {
      std::cout << arg << " is a shell builtin" << std::endl;
    } else {
      bool found = false;
      for (const auto &path : paths) {
        std::string exec_path = path + '/' + arg;
        if (find_executable_path(exec_path)) {
          std::cout << arg << " is " << exec_path << std::endl;
          found = true;
          break;
        }
      }
      if (!found) {
        std::cout << arg << ": not found" << std::endl;
      }
    }
  } else if (command == "pwd") {
    std::system(command.c_str());
  } else if (command.starts_with("cd")) {
    std::string dir_path = command.substr(3);
    if (fs::exists(dir_path) && fs::is_directory(dir_path)) {
      CHDIR(dir_path.c_str());
    }
    else {
      std::cout << "cd: " << dir_path << ": No such file or directory" << std::endl;
    }
  } else {
    bool found = false;
    std::string exec_name = command.substr(0, command.find(' '));
    for (const auto &path : paths) {
      std::string exec_path = path + '/' + exec_name;
      if (find_executable_path(exec_path)) {
        std::system(command.c_str());
        found = true;
        break;
      }
    }
    if (!found) {
      std::cout << command << ": command not found" << std::endl;
    }
  }

  main();
}
