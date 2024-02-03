import itertools
with open('../input_17_1.txt') as file:
    containers = [int(line.strip()) for line in file]
total = 0
for num in range(len(containers)):
    for combination in itertools.combinations(containers, num):
        if sum(combination) == 150:
            total += 1
print(total)
