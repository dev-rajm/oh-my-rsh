#include "utils.h"
#include <filesystem>
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
