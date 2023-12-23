import numpy as np

def command(s: str, arr: np.array):
	action = 0
	start = 0
	key_letter = s[6]
	if  key_letter == 'n':
		start = 8
		action = 1
	elif key_letter == 'f':
		start = 9
		action = -1
	else:
		start = 7
		action = 2

	groups = s[start:].split(' through ')
	f1,f2 = [int(val) for val in groups[0].split(',')]
	t1,t2 = [int(val) for val in groups[1].split(',')]

	arr[f1:t1 + 1,f2:t2 + 1] += action
	arr[arr < 0] = 0


grid = np.zeros((1000,1000),dtype=int)

with open('../input_06_1.txt', 'r') as f:
	for line in f.readlines():
		command(line, grid)
	print(grid.sum())
