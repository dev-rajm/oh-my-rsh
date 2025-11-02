#include <iostream>
#include <string>

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "$ ";

  std::string command;
  std::getline(std::cin, command);
  if(command == "exit 0") {
    return 0;
  }
  std::cout << command << ": command not found" << std::endl;

  main();
}
