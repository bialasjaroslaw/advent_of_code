def count_characters(line: str):
	if len(line) < 2:
		raise ValueError(f"Invalid string passed {line}")
	line = line[1:-1]
	escaped = False
	hex_seq = 0
	count = 0
	total_hex = 0
	total_escaped = 0
	for letter in line:
		if escaped:
			if 0 < hex_seq < 2:
				hex_seq += 1
			elif letter == "x":
				hex_seq = 1
			else:
				if hex_seq != 0:
					total_hex += 1
				else:
					total_escaped += 1
				count += 1
				hex_seq = 0
				escaped = False
			continue
		elif letter == "\\":
			escaped = True
			continue
		count += 1
	return count, total_escaped, total_hex

def calc_additional_chars(line: str):
	_, escaped, hex_seq = count_characters(line)
	return 4 + 2 * escaped + hex_seq


with open('../input_08_1.txt', 'r') as f:
	print(sum(calc_additional_chars(line) for line in f.read().split('\n') if line != ""))

