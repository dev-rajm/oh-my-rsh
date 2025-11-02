#include <iostream>
#include <string>
#include <vector>

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

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
      std::cout << arg << ": not found" << std::endl;
    }
  } else {
    std::cout << command << ": command not found" << std::endl;
  }

  main();
}
