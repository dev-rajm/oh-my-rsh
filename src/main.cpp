#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

#ifdef _WIN32
const char DELIMITER = ';';
#else
const char DELIMITER = ':';
#endif

std::vector<std::string> split(char *input) {
  std::vector<std::string> paths;
  std::string path;
  std::stringstream ss(input);

  while (std::getline(ss, path, DELIMITER)) {
    paths.emplace_back(path);
  }

  return paths;
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // get the path environment variable
  char *path_value = getenv("PATH");
  std::vector<std::string> splited_paths = split(path_value);

  std::vector<std::string> builtins{"type", "exit", "echo"};

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
      for (const auto &path : splited_paths) {
        std::string fpath = path + '/' + arg;
        if (fs::exists(fpath)) {
          found = true;
          fs::file_status fstatus = fs::status(fpath);
          fs::perms fpermission = fstatus.permissions();
          if ((fpermission & fs::perms::owner_exec) != fs::perms::none) {
            std::cout << arg << " is " << fpath << std::endl;
            break;
          }
        }
      }
      if(!found) {
        std::cout << arg << ": not found" << std::endl;
      }
    }
  } else {
    std::cout << command << ": command not found" << std::endl;
  }

  main();
}
