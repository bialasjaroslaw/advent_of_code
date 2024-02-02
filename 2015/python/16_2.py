from typing import Dict, Tuple, List
import math

class Aunt:
    def __init__(self, number, **kwargs):
        self.number = number
        self.attributes = {**kwargs}

    @staticmethod
    def from_str(line: str):
        idx = line.index(':')
        number = int(line[4:idx])
        parts = line[idx + 2:].strip().split(", ")
        gen = (part.strip().split(': ') for part in parts)
        props = {part[0]: int(part[1]) for part in gen}
        return Aunt(number, **props)

    def __repr__(self):
        return f"Aunt({self.number}, {self.attributes})"

    def has_item(self, name, value):
        if name not in self.attributes:
            return True
        if name in ["cats", "trees"]:
            return self.attributes[name] > value
        elif name in ["pomeranians", "goldfish"]:
            return self.attributes[name] < value
        return self.attributes[name] == value
        

    def has(self, things):
        return all(self.has_item(key, amount) for key, amount in things.items())

with open('../input_16_1.txt') as file:
    aunts = [Aunt.from_str(line) for line in file]

should_have = {
    "children" : 3,
    "cats" : 7,
    "samoyeds" : 2,
    "pomeranians" : 3,
    "akitas" : 0,
    "vizslas" : 0,
    "goldfish" : 5,
    "trees" : 3,
    "cars" : 2,
    "perfumes" : 1,
}

print(next((aunt for aunt in aunts if aunt.has(should_have)), None).number)
