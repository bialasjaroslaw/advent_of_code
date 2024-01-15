import more_itertools

def expand_seq(seq: str):
	return str(len(seq)) + seq[0]


def split_to_seq(line: str):
	return more_itertools.split_when(line, lambda x,y: x != y)


def expand(line: str):
	return ''.join((expand_seq(seq) for seq in split_to_seq(line)))

with open('../input_10_1.txt', 'r') as f:
	line = f.read().strip()
	for _ in range(50):
		line = expand(line)
	print(len(line))
