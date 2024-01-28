#include <fmt/printf.h>
#include <io_dev/file.h>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/filter.hpp>
#include <text_processing/count.hpp>

uint64_t encode_space(const std::string &str) {
  return str.size() + static_cast<uint64_t>(Text::count_any(str, "\"\\") + 2);
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
      [](const auto &line) { return encode_space(line) - line.size(); });
  fmt::print("{}\n", total);

  return 0;
}
