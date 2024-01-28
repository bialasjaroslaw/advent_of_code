#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/view/transform.hpp>
#include <text_processing/convert.hpp>
#include <text_processing/split.hpp>
#include <text_processing/sub.hpp>
#include <vector>

struct Reindeer {
  std::string name;
  uint32_t speed = 0;
  uint32_t duration = 0;
  uint32_t reset = 0;
};

Reindeer make_reindeer(const std::string &line) {
  if (line.size() == 0)
    return {};
  auto parts = Text::split(Text::left_view(line, -1), " ");
  if (parts.size() != 15)
    return {};
  auto name = std::string(parts[0]);
  auto speed = Text::convert<uint32_t>(parts[3]);
  auto duration = Text::convert<uint32_t>(parts[6]);
  auto resting = Text::convert<uint32_t>(parts[13]);

  return Reindeer{name, speed, duration, resting};
}

uint64_t distance(const Reindeer &deer, uint32_t time) {
  auto slot_time = deer.duration + deer.reset;
  auto slots = time / slot_time;
  auto left = std::min(time - slot_time * slots, deer.duration);
  return (left + slots * deer.duration) * deer.speed;
}

uint64_t fastest_reindeer(const std::vector<Reindeer> &reindeers,
                          uint32_t time) {
  return ranges::max(reindeers |
                     ranges::views::transform([time](const auto &elem) {
                       return distance(elem, time);
                     }));
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
  std::vector<Reindeer> reindeers;
  for (const auto &line : lines) {
    auto deer = make_reindeer(line);
    if (deer.speed > 0)
      reindeers.push_back(deer);
  }
  fmt::print("{}\n", fastest_reindeer(reindeers, 2503));
  return 0;
}
