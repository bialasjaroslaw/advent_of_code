import functools

def process_line(line: str):
	w, h, l = [int(val) for val in line.split('x')]
	m = max(w, h, l)
	around = 2 * (w + h + l - m)
	bow = w * h * l
	return around + bow

dims = open('../input_02_1.txt').readlines()

print(functools.reduce(lambda p1, p2: p1 + p2, [process_line(line) for line in dims]))