#include "utils.h"
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

    if (command.empty())
      continue;
    if (command == "exit" || command == "exit 0")
      std::exit(0);

    const std::string command_name = command.substr(0, command.find(' '));
    const std::string args_part = command.substr(command.find(' ') + 1);

    handle_command(command_name, args_part, command, builtins, paths);
  }
}
