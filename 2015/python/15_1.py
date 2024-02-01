from typing import Dict, Tuple, List
import math

class Ingredient:
    def __init__(self, capacity: int, durability: int, flavor: int, texture: int, calories: int):
        self.values = [capacity, durability, flavor, texture, calories]

    @staticmethod
    def from_str(line: str):
        parts = line.strip().split(": ")
        chunks = parts[1].split(",")
        gen = (chunk.strip().split(' ') for chunk in chunks)
        props = {chunk[0]: int(chunk[1]) for chunk in gen}
        return Ingredient(**props)

ingredients = []

def taste_score(ingredients: List[Ingredient], qty: List[int]):
    par = [0] * 4
    for ing, q in zip(ingredients, qty):
        par = [p + v * q for p,v in zip(par, ing.values)]
    if any((p <= 0 for p in par)):
        return 0
    return par[0] * par[1] * par[2] * par[3]

with open('../input_15_1.txt') as file:
    ingredients = [Ingredient.from_str(line) for line in file]

num = len(ingredients)
num_m = num - 1
best_score = 0
qty = [0] * num 

for val in range(100**num_m):
    for idx in range(num_m):
        qty[idx] = val % 100
        val = (val - qty[idx]) // 100
    qty[num_m] = 100 - sum(qty[0:num_m])
    if qty[num_m] < 0:
        continue

    score = taste_score(ingredients, qty)
    if best_score < score:
        best_score = score

print(best_score)