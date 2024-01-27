import itertools
from typing import Dict, Tuple, List
from functools import partial

def parse_input(line: str, p: List[str], d: Dict[Tuple[str, str], int]):
    parts = line.strip()[:-1].split(" ")
    value = int(parts[3])
    value = value if parts[2] == "gain" else -value
    me = parts[0]
    if me not in people:
        p.append(me)
    neighbor = parts[-1]
    d[(me, neighbor)] = value

def calc_happiness(order, relations):
    length = len(order)
    neighbour_happiness = lambda idx, n_idx: relations[(idx, n_idx)] + relations[(n_idx, idx)]
    return sum((neighbour_happiness(order[n], order[(n + 1) % length]) for n in range(length)))

people = []
relations = {}

with open('../input_13_1.txt') as file:
    for line in file:
        parse_input(line, people, relations)
for person in people:
    relations[("me", person)] = relations[(person, "me")] = 0
people.append("me")

print(max(map(partial(calc_happiness, relations=relations), itertools.permutations(people))))
