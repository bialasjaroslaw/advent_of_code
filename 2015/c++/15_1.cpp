#include "text_processing/convert.hpp"
#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <io_dev/file.h>
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
           Text::convert<int32_t>(parts[5]), Text::convert<int32_t>(parts[7])}};
}

void select_starting_point(std::vector<Ingredient> &ingredients, int32_t max) {
  auto part_score = max / static_cast<int32_t>(ingredients.size());
  auto rest = max - part_score * static_cast<int32_t>(ingredients.size() - 1);
  for (auto &item : ingredients) {
    item.quantity = part_score;
  }
  ingredients[0].quantity = rest;
}

using ing_vec = std::vector<Ingredient>;

int64_t score(const ing_vec &ingredients) {
  std::vector<int64_t> partials(ingredients.size(), 0);
  std::for_each(ingredients.cbegin(), ingredients.cend(),
                [&partials](const auto &item) {
                  for (uint64_t idx = 0; idx < partials.size(); ++idx) {
                    partials[idx] += item.scores[idx] * item.quantity;
                  }
                });
  int64_t mul = std::max(0L, partials[0]);
  for (uint64_t idx = 1; idx < partials.size(); ++idx) {
    if (partials[idx] < 0) {
      mul = 0;
      break;
    }
    mul *= partials[idx];
  }
  return mul;
}

bool maximize_on_idx(ing_vec &ingredients, uint64_t idx, uint64_t next_idx,
                     int64_t max) {
  auto starting_q = ingredients[idx].quantity;
  auto starting_q_n = ingredients[next_idx].quantity;
  auto best_score = score(ingredients);
  auto best_idx = starting_q;
  auto best_next_idx = ingredients[next_idx].quantity;
  for (auto qnt = starting_q + 1; qnt <= max; ++qnt) {
    if (ingredients[next_idx].quantity > 0) {
      ++ingredients[idx].quantity;
      --ingredients[next_idx].quantity;
    }
    auto new_score = score(ingredients);
    if (new_score > best_score) {
      best_score = new_score;
      best_idx = ingredients[idx].quantity;
      best_next_idx = ingredients[next_idx].quantity;
    } else {
      break;
    }
  }
  ingredients[idx].quantity = starting_q;
  ingredients[next_idx].quantity = starting_q_n;
  for (auto qnt = starting_q - 1; qnt >= 0; --qnt) {
    if (ingredients[next_idx].quantity < max) {
      --ingredients[idx].quantity;
      ++ingredients[next_idx].quantity;
    }
    auto new_score = score(ingredients);
    if (new_score > best_score) {
      best_score = new_score;
      best_idx = ingredients[idx].quantity;
      best_next_idx = ingredients[next_idx].quantity;
    } else {
      break;
    }
  }

  ingredients[idx].quantity = best_idx;
  ingredients[next_idx].quantity = best_next_idx;
  return starting_q != best_idx;
}

int64_t maximize(ing_vec &ingredients) {
  bool updated = true;
  while (updated) {
    updated = false;
    for (uint64_t idx = 0; idx < ingredients.size(); ++idx) {
      for (uint64_t next_idx = 0; next_idx < ingredients.size(); ++next_idx) {
        if (idx == next_idx)
          continue;
        auto changed = maximize_on_idx(ingredients, idx,
                                       (idx + 1) % ingredients.size(), 100);
        if (changed)
          updated = true;
      }
    }
  }
  return score(ingredients);
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
  select_starting_point(ingredients, 100);
  fmt::print("{}\n", maximize(ingredients));
  return 0;
}
