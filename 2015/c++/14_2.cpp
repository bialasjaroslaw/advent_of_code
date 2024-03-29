#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/view/filter.hpp>
#include <sys/types.h>
#include <text_processing/convert.hpp>
#include <text_processing/split.hpp>
#include <text_processing/sub.hpp>
#include <vector>

struct Reindeer {
  std::string name;
  uint32_t speed = 0;
  uint32_t duration = 0;
  uint32_t reset = 0;
  uint32_t current = 0;
  uint32_t time = 0;
  uint32_t score = 0;
  bool resting = false;

  void tick() {
    ++time;
    if (!resting) {
      current += speed;
      if (time == duration) {
        time = 0;
        resting = true;
      }
    } else {
      if (time == reset) {
        time = 0;
        resting = false;
      }
    }
  }

  void add_score() { ++score; }
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

  for (uint32_t sec = 0; sec < 2503; ++sec) {
    ranges::for_each(reindeers, &Reindeer::tick);
    auto best_deer = ranges::max(reindeers, std::less<>(), &Reindeer::current);
    ranges::for_each(
        reindeers | ranges::views::filter([value = best_deer.current](
                                              auto v) { return v == value; },
                                          &Reindeer::current),
        &Reindeer::add_score);
  }
  auto best_deer = ranges::max(reindeers, std::less<>(), &Reindeer::score);
  fmt::print("{}\n", best_deer.score);
  return 0;
}
