def update_pos(char, position):
	if c == '<':
		position[1] -= 1
	elif c == '>':
		position[1] += 1
	elif c == '^':
		position[0] += 1
	else:
		position[0] -= 1
	return position

def update_gifts(position, gifts):
	key = tuple(position)
	if key not in gifts:
		gifts[key] = 0
	gifts[key] += 1


gifts = dict()
with open('../input_03_1.txt', 'r') as f:
	santa_pos = [0, 0]
	update_gifts(santa_pos, gifts)
	for c in f.read():
		update_gifts(update_pos(c, santa_pos), gifts)

print(len(gifts))

