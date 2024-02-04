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
bound = next((idx for idx, value in next_sum(containers) if value >= 150), None)
min_num = next((num for num in range(bound) for combination in itertools.combinations(containers, num) if sum(combination) == 150), None)
print(sum(sum(combination) == 150 for combination in itertools.combinations(containers, min_num)))