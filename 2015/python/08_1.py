def count_characters(line: str):
	if len(line) < 2:
		raise ValueError(f"Invalid string passed {line}")
	line = line[1:-1]
	escaped = False
	hex_seq = 0
	count = 0
	for letter in line:
		if escaped:
			if 0 < hex_seq < 2:
				hex_seq += 1
			elif letter == "x":
				hex_seq = 1
			else:
				count += 1
				hex_seq = 0
				escaped = False
			continue
		elif letter == "\\":
			escaped = True
			continue
		count += 1
	return count

with open('../input_08_1.txt', 'r') as f:
	print(sum(len(line) - count_characters(line) for line in f.read().split('\n') if line != ""))

