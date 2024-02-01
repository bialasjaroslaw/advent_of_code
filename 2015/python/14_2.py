from typing import Dict, Tuple, List
import math

class Reindeer:
    def __init__(self, name: str, speed: int, duration: int, rest: int):
        self.name = name
        self.speed = speed
        self.duration = duration
        self.rest = rest
        self.running = True
        self.time = 0
        self.total = 0
        self.score = 0

    def tick(self):
        self.time += 1
        if self.running:
            self.total += self.speed
            if self.time == self.duration:
                self.running = False
                self.time = 0
        else:
            if self.time == self.rest:
                self.running = True
                self.time = 0

    @staticmethod
    def from_str(line: str):
        parts = line.split(" ")
        return Reindeer(parts[0], int(parts[3]), int(parts[6]), int(parts[13]))

def simulate(deers: List[Reindeer], time: int):
    for _ in range(time):
        for deer in deers:
            deer.tick()
        longest_dist = max(map(lambda x: x.total, reindeers))
        for deer in deers:
            if deer.total == longest_dist:
                deer.score += 1

reindeers = []

with open('../input_14_1.txt') as file:
    reindeers = [Reindeer.from_str(line) for line in file]
simulate(reindeers, 2503)
print(max(map(lambda x: x.score, reindeers)))
