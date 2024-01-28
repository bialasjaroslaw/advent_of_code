#include <fmt/printf.h>
#include <io_dev/file.h>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/filter.hpp>

uint64_t decode_space(const std::string &str) {
  if (str.size() < 2)
    return 0;
  uint64_t len = 0;
  bool sequence = false;
  for (uint64_t idx = 1; idx < str.size() - 1; ++idx) {
    if (str[idx] == '\\') {
      if (sequence)
        ++len;
      sequence = !sequence;
    } else if (sequence && str[idx] == 'x') {
      sequence = false;
      ++len;
      idx += 2;
    } else {
      sequence = false;
      ++len;
    }
  }
  return len;
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
  auto maybe_lines = IO::File(path, IO::OpenMode::Read).read_lines();
  if (!maybe_lines) {
    fmt::print("Can not read lines from {}\n", path.string());
    return -1;
  }
  auto lines = maybe_lines.value();
  auto total = ranges::accumulate(
      lines | ranges::views::filter([](const auto &s) { return !s.empty(); }),
      0UL, std::plus<>(),
      [](const auto &line) { return line.size() - decode_space(line); });
  fmt::print("{}\n", total);

  return 0;
}
