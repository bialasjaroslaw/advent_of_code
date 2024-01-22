#include <cstdint>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <string>
#include <vector>

struct char_info {
  char letter = '\0';
  uint32_t cnt = 0;
};

std::string transform_word(const std::string &word) {
  auto prev_char = '\0';
  std::vector<char_info> info;
  info.reserve(word.size() / 2);
  uint32_t cnt = 0;
  uint64_t out_len = 0;
  for (uint32_t idx = 0; idx < word.size(); ++idx) {
    if (prev_char != '\0' && prev_char != word[idx]) {
      info.emplace_back(prev_char, cnt);
      out_len += (1 + cnt / 10);
      cnt = 0;
    }
    ++cnt;
    prev_char = word[idx];
  }
  info.emplace_back(prev_char, cnt);
  out_len += (1 + cnt / 10);

  std::string str;
  str.reserve(out_len);
  for (const auto &inf : info) {
    str.append(std::to_string(inf.cnt));
    str.push_back(inf.letter);
  }

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

  for (uint64_t idx = 0; idx < 40; ++idx) {
    line = transform_word(line);
  }
  fmt::print("{}\n", line.size());

  return 0;
}
