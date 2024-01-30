#include "text_processing/convert.hpp"
#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
#include <limits>
#include <text_processing/split.hpp>

struct Ingredient {
  std::vector<int64_t> scores{};
  int64_t quantity = 0;
};

Ingredient make_ingriedient(const std::string &str) {
  auto ingredients = Text::split(str, ": ");
  if (ingredients.size() != 2)
    return {};
  auto parts =
      Text::split_any(ingredients[1], ", ", Text::SplitBehavior::DropEmpty);
  if (parts.size() != 10) {
    return {};
  }

  return {{Text::convert<int32_t>(parts[1]), Text::convert<int32_t>(parts[3]),
           Text::convert<int32_t>(parts[5]), Text::convert<int32_t>(parts[7]),
           Text::convert<int32_t>(parts[9])}};
}

using ing_vec = std::vector<Ingredient>;

int64_t calories(const ing_vec &ingredients) {
  int32_t total = 0;
  for (const auto &item : ingredients) {
    total += item.quantity * item.scores[4];
  }
  return total;
}

int64_t score(const ing_vec &ingredients) {
  std::vector<int64_t> partials(4, 0);
  std::for_each(ingredients.cbegin(), ingredients.cend(),
                [&partials](const auto &item) {
                  for (uint64_t idx = 0; idx < partials.size(); ++idx) {
                    partials[idx] += item.scores[idx] * item.quantity;
                  }
                });
  int64_t mul = 1;
  for (uint64_t idx = 0; idx < partials.size(); ++idx) {
    if (partials[idx] <= 0) {
      mul = 0;
      break;
    }
    mul *= partials[idx];
  }
  return mul;
}

bool next_comb(ing_vec &ingredients) {
  int32_t sum = 101;
  auto start = ingredients[1].quantity;
  while (sum > 100) {
    bool carry = true;
    sum = 0;
    for (uint64_t idx = 3; idx > 0; idx--) {
      if (carry) {
        ++ingredients[idx].quantity;
        if (ingredients[idx].quantity > 100) {
          ingredients[idx].quantity = 0;
          continue;
        } else {
          carry = false;
        }
      }
      sum += ingredients[idx].quantity;
    }
  }
  ingredients[0].quantity = 100 - sum;
  return start <= ingredients[1].quantity;
}

int64_t maximize(ing_vec &ingredients) {
  auto best_score = std::numeric_limits<int32_t>::min();
  while (next_comb(ingredients)) {
    if (calories(ingredients) != 500)
      continue;
    auto current_score = score(ingredients);
    if (current_score > best_score) {
      best_score = current_score;
    }
  }
  return best_score;
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
  std::vector<Ingredient> ingredients;
  for (const auto &line : lines) {
    auto ingredient = make_ingriedient(line);
    if (ingredient.scores.size() > 0) {
      ingredients.push_back(ingredient);
    }
  }
  fmt::print("{}\n", maximize(ingredients));
  return 0;
}
