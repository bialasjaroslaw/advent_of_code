#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <limits>
#include <string>
#include <text_processing/convert.hpp>
#include <text_processing/split.hpp>
#include <text_processing/sub.hpp>
#include <unordered_map>
#include <vector>

std::vector<std::string> people;
std::unordered_map<std::string, std::unordered_map<std::string, int>> happines;

void add_person_info(const std::string &str) {
  auto parts = Text::split(Text::left_view(str, -1), " ");
  if (parts.size() != 11)
    return;
  auto name = std::string(parts[0]);
  auto value = Text::convert<int>(parts[3]);
  value = parts[2] == "gain" ? value : -value;
  auto whom = std::string(parts[10]);
  happines[name][whom] = value;
}

int64_t calc_happiness_value(const std::vector<std::string> seating) {
  int64_t total = 0;
  for (uint64_t idx = 0; idx < seating.size(); ++idx) {
    auto next_idx = idx == seating.size() - 1 ? 0 : idx + 1;
    total += (happines[seating[next_idx]][seating[idx]] +
              happines[seating[idx]][seating[next_idx]]);
  }
  return total;
}

auto find_best_seating() {
  for (const auto &p : happines) {
    people.push_back(p.first);
  }
  std::sort(people.begin(), people.end());
  auto best_seating = people;
  auto best_happines = std::numeric_limits<int64_t>::min();
  do {
    auto current = calc_happiness_value(people);
    // print_scenario(people, current);
    if (current > best_happines) {
      best_happines = current;
      best_seating = people;
    }
  } while (std::next_permutation(people.begin(), people.end()));
  return best_happines;
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
  for (const auto &line : lines) {
    add_person_info(line);
  }
  fmt::print("{}\n", find_best_seating());

  return 0;
}
