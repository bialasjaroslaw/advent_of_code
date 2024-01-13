import itertools
from typing import List

routes = dict()

def add_route(routes: dict, starting: str, ending: str, distance: int):
	if starting not in routes:
		routes[starting] = dict()
	routes[starting][ending] = distance
	if ending not in routes or starting not in routes[ending]:
		add_route(routes, ending, starting, distance)


def calculate_route_len(cities: List[str]):
	return sum((routes[city_a][city_b] for city_a, city_b in zip(cities[:-1], cities[1:])))



with open('../input_09_1.txt', 'r') as f:
	for line in f:
		line = line.strip()
		if line == "":
			continue
		cities, distance = line.split(' = ')
		city_a, city_b = cities.split(' to ')
		add_route(routes, city_a, city_b, int(distance))

	cities = routes.keys()
	print(max(map(calculate_route_len, itertools.permutations(cities, len(cities)))))
