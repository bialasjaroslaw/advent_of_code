#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numeric>
#include <string>
#include <text_processing/count.hpp>

uint64_t count(const nlohmann::json &json_obj) {
  if (json_obj.is_number())
    return json_obj.get<uint64_t>();
  if (json_obj.is_array() || json_obj.is_object())
    if (!json_obj.is_object() ||
        std::all_of(
            json_obj.begin(), json_obj.end(), [](const nlohmann::json &item) {
              return !item.is_string() || item.get<std::string_view>() != "red";
            }))
      return std::accumulate(json_obj.begin(), json_obj.end(), 0U,
                             [](uint64_t acc, const auto &current) {
                               return acc + count(current);
                             });
  return 0;
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
  auto json = nlohmann::json::parse(line);
  fmt::print("{}\n", count(json));
  return 0;
}
