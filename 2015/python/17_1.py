import itertools

def next_sum(container):
    elem_sum = 0
    for idx, elem in enumerate(container):
        elem_sum += elem
        yield idx, elem_sum
    raise StopIteration()

with open('../input_17_1.txt') as file:
    containers = [int(line.strip()) for line in file]

containers.sort()
lower_bound = next((idx for idx, value in next_sum(reversed(containers)) if value >= 150), None)
upper_bound = next((idx for idx, value in next_sum(containers) if value > 150), None) + 2
print(sum(sum(combination) == 150 for num in range(lower_bound, upper_bound) for combination in itertools.combinations(containers, num)))
