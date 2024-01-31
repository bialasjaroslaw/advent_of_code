from typing import Dict, Tuple, List
import math

class Reindeer:
    def __init__(self, name: str, speed: int, duration: int, rest: int):
        self.name = name
        self.speed = speed
        self.duration = duration
        self.rest = rest

    @staticmethod
    def from_str(line: str):
        parts = line.split(" ")
        return Reindeer(parts[0], int(parts[3]), int(parts[6]), int(parts[13]))

def calc_distance(reindeer: Reindeer, time: int):
    slot = (reindeer.duration + reindeer.rest)
    whole = int(math.floor(time / slot))
    rest = time - whole * slot
    partial = min(rest, reindeer.duration)
    return (whole * reindeer.duration + partial) * reindeer.speed

reindeers = []

with open('../input_14_1.txt') as file:
    reindeers = [Reindeer.from_str(line) for line in file]

print(max(map(lambda x: calc_distance(x, 2503), reindeers)))
