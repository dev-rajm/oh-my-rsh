#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "../headers/utils.h"

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> builtins{"type", "exit", "echo", "pwd"};
  
  char *process_env = getenv("PATH");
  std::vector<std::string> paths = split(process_env);

  std::cout << "$ ";

  std::string command;
  std::getline(std::cin, command);
  if (command == "exit 0") {
    return 0;
  }

  if (command.starts_with("echo")) {
    std::vector<std::string> str = split_input(command.substr(5));
    for(size_t i=0; i<str.size(); i++) {
      std::cout << str[i];
      if(i+1 < str.size()) {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
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
    if (dir_path == "~")
      dir_path = std::getenv("HOME");
    if (find_executable_path(dir_path) && fs::is_directory(dir_path)) {
      CHDIR(dir_path.c_str());
    } else {
      std::cout << "cd: " << dir_path << ": No such file or directory"
                << std::endl;
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
