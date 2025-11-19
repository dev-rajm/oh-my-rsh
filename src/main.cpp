#include "utils.h"
#include <sstream>
#include <iostream>

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> builtins{"type", "exit", "echo", "pwd"};
  const std::string process_env = std::getenv("PATH");
  std::vector<std::string> paths = split(process_env);

  while (true) {
    std::cout << "$ ";

    std::string command;
    if (!std::getline(std::cin, command))
      return 0;

    if (command.find_first_not_of(" \t") == std::string::npos)
      continue;

    trim(command);

    if (command == "exit" || command == "exit 0")
      std::exit(0);

    std::string command_name;
    std::string args_part;

    {
      std::istringstream iss(command);
      iss >> command_name;
      std::getline(iss, args_part);
      trim(args_part);
    }

    handle_command(command_name, args_part, command, builtins, paths);
  }
}
