#include <algorithm>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <limits>
#include <string>
#include <text_processing/convert.hpp>
#include <text_processing/split.hpp>
#include <unordered_map>
#include <vector>

std::vector<std::string> cities;
std::unordered_map<std::string, std::unordered_map<std::string, int>> distances;

void add_city_info(const std::string &str) {
  auto parts = Text::split(str, " = ");
  if (parts.size() != 2)
    return;
  auto point_to_point = parts[0];
  auto distance = Text::convert<int>(parts[1]);
  auto cities = Text::split(point_to_point, " to ");
  auto from_city = std::string(cities[0]);
  auto to_city = std::string(cities[1]);
  distances[from_city][to_city] = distance;
  distances[to_city][from_city] = distance;
}

int calc_route_len(const std::vector<std::string> route) {
  auto total = 0;
  for (int idx = 0; idx < route.size() - 1; ++idx) {
    total += distances[route[idx]][route[idx + 1]];
  }
  return total;
}

auto find_best_route() {
  for (const auto &p : distances)
    cities.push_back(p.first);
  auto best_path = cities;
  auto best_len = std::numeric_limits<int>::max();
  do {
    auto current = calc_route_len(cities);
    if (current < best_len) {
      best_len = current;
      best_path = cities;
    }
  } while (std::next_permutation(cities.begin(), cities.end()));
  return best_len;
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
    add_city_info(line);
  }
  fmt::print("{}\n", find_best_route());

  return 0;
}
