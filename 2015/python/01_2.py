floors = open('../input_01_1.txt').read()
current_floor = 0
for idx, direction in enumerate(floors):
	current_floor += 1 if direction == '(' else -1
	if current_floor == -1:
		print(idx + 1)
		break
