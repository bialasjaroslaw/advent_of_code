import functools

def process_line(line: str):
	w, h, l = [int(val) for val in line.split('x')]
	m = max(w, h, l)
	additional = int((w * h * l) / m)
	return 2 * (w*h + h*l + l*w) + additional

dims = open('../input_02_1.txt').readlines()

print(functools.reduce(lambda p1, p2: p1 + p2, [process_line(line) for line in dims]))