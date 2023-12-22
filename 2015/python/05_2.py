import string

vowels = {'a', 'e', 'i', 'o', 'u'}

def is_nice_string(s: str):
	three_seq = False
	for idx in range(len(s) - 2):
		if s[idx + 2] == s[idx]:
			three_seq = True
			break
	if not three_seq:
		return False

	pair_seq = False
	for idx in range(len(s) - 3):
		pair = s[idx:idx + 2]
		if s.find(pair, idx + 2) != -1:
			pair_seq = True
			break
	if not pair_seq:
		return False
	return True


with open('../input_05_1.txt', 'r') as f:
	nice = 0
	for line in f.readlines():
		if is_nice_string(line):
			nice += 1
	print(nice)

