#include "utils.h"
#include <filesystem>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

std::vector<std::string> split(std::string input, char divider) {
  std::vector<std::string> chunks;
  std::string chunk;
  std::stringstream ss(input);

  while (std::getline(ss, chunk, divider)) {
    chunks.emplace_back(chunk);
  }

  return chunks;
}

std::vector<std::string> split_input(const std::string &str) {
  std::vector<std::string> tokens;

  bool in_single_quotes = false;
  bool in_double_quotes = false;
  bool in_escape_literal = false;
  std::string curr_token = "";

  for (auto ch : str) {
    if (ch == '"' && !in_single_quotes && !in_escape_literal) {
      in_double_quotes = !in_double_quotes;
    } else if (ch == '\'' && !in_double_quotes && !in_escape_literal) {
      in_single_quotes = !in_single_quotes;
    } else if (ch == '\\' && !in_escape_literal && !in_single_quotes) {
      in_escape_literal = !in_escape_literal;
    } else if (std::isspace(ch) && !in_escape_literal && !in_single_quotes &&
               !in_double_quotes) {
      if (!curr_token.empty()) {
        tokens.push_back(curr_token);
        curr_token.clear();
      }
    } else {
      curr_token += ch;
      if (in_escape_literal) {
        in_escape_literal = false;
      }
    }
  }

  if (!curr_token.empty()) {
    tokens.push_back(curr_token);
  }

  return tokens;
}

bool find_executable_path(const std::string &exec_path) {
  if (fs::exists(exec_path)) {
    fs::file_status exec_status = fs::status(exec_path);
    fs::perms exec_perm = exec_status.permissions();
    if ((exec_perm & fs::perms::owner_exec) != fs::perms::none) {
      return true;
    }
  }
  return false;
}

// ----- echo -----
void handle_echo(const std::vector<std::string> &input) {
  for (size_t i = 0; i < input.size(); i++) {
    std::cout << input[i];
    if (i + 1 < input.size()) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;
}

// ----- type -----
void handle_type(const std::string &input,
                 const std::vector<std::string> &builtins,
                 const std::vector<std::string> &paths) {
  if (std::find(builtins.begin(), builtins.end(), input) != builtins.end()) {
    std::cout << input << " is a shell builtin" << std::endl;
    return;
  }
  for (const auto &path : paths) {
    std::string exec_path = path + '/' + input;
    if (find_executable_path(exec_path)) {
      std::cout << input << " is " << exec_path << std::endl;
      return;
    }
  }
  std::cerr << input << ": not found" << std::endl;
}

// ----- pwd -----
void handle_pwd() { std::cout << fs::current_path().string() << std::endl; }

// ----- cd -----
void handle_cd(const std::string &input) {
  std::string dir = input;
  if (dir == "~")
    dir = std::getenv("HOME") ? std::getenv("HOME") : "/";
  if (fs::exists(dir) && fs::is_directory(dir)) {
    if (CHDIR(dir.c_str()) != 0) {
      std::perror("cd");
    }
  } else {
    std::cerr << "cd: " << dir << ": No such file or directory" << std::endl;
  }
}

// ----- external -----
void handle_external(const std::string &input,
                     const std::vector<std::string> &paths) {
  std::string exec_name;
  if (input.front() == '\'')
    exec_name = input.substr(1, input.find_last_of('\'') - 1);
  else if (input.front() == '"')
    exec_name = input.substr(1, input.find_last_of('"') - 1);
  else
    exec_name = input.substr(0, input.find(' '));
  for (const auto &path : paths) {
    std::string exec_path = path + '/' + exec_name;
    if (find_executable_path(exec_path)) {
      std::system(input.c_str());
      return;
    }
  }
  std::cerr << exec_name << ": command not found" << std::endl;
}

// ----- handle commands -----
void handle_command(const std::string &command_name, const std::string &args,
                    const std::string &command,
                    const std::vector<std::string> &builtins,
                    const std::vector<std::string> &paths) {
  if (command_name == "echo") {
    handle_echo(split_input(args));
  } else if (command_name == "type") {
    handle_type(args, builtins, paths);
  } else if (command_name == "pwd") {
    handle_pwd();
  } else if (command_name == "cd") {
    handle_cd(args);
  } else {
    handle_external(command, paths);
  }
}
