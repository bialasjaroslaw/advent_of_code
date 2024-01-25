#include "text_processing/common.hpp"
#include <cstdint>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <string>
#include <text_processing/find.hpp>

bool increasing_seq(const std::string &str) {
  for (uint64_t idx = 0; idx < str.size() - 2; ++idx) {
    if (str[idx] + 1 == str[idx + 1] && str[idx + 1] + 1 == str[idx + 2])
      return true;
  }
  return false;
}

bool no_forbidden_letters(const std::string &str) {
  std::string_view forbidden("iol");
  return Text::find_any(str, forbidden) == Text::End;
}

uint64_t pair_of_letters(const std::string &str, uint64_t idx = 0) {
  for (; idx < str.size() - 1; ++idx) {
    if (str[idx] == str[idx + 1])
      return idx + 2;
  }
  return 0;
}

bool two_pairs_of_letter(const std::string &str) {
  auto first_pair = pair_of_letters(str);
  if (first_pair == 0)
    return false;
  auto second_pair = pair_of_letters(str, first_pair);
  return second_pair != 0;
}

void next_password(std::string &str) {
  int64_t carry = 1;
  for (uint64_t idx = str.size(); idx > 0; --idx) {
    if (carry == 0)
      break;
    carry = 0;
    ++str[idx - 1];
    if (str[idx - 1] > 'z') {
      carry = 1;
      str[idx - 1] = 'a';
    }
  }
}

const std::string &update_password(std::string &str) {
  do {
    next_password(str);
  } while (!increasing_seq(str) || !no_forbidden_letters(str) ||
           !two_pairs_of_letter(str));
  return str;
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    fmt::print("Not enough arguments\n");
    return 1;
  }
  auto path = std::filesystem::path(argv[1]);
  if (!std::filesystem::exists(path)) {
    fmt::print("File not exists\n");
    return 2;
  }
  auto maybe_line = IO::File(path, IO::OpenMode::Read).read();
  if (!maybe_line) {
    fmt::print("Can not read lines from {}\n", path.string());
    return -1;
  }
  auto line = maybe_line.value();
  fmt::print("{}\n", update_password(line));
  fmt::print("{}\n", update_password(line));
  return 0;
}
