import functools

def extract_two_digits(s: str):
	ll = list(filter(lambda x: x.isdigit(), s))
	return 10 * int(ll[0]) + int(ll[-1])


lst = open('../input_01_1.txt').readlines()
value = functools.reduce(lambda p1, p2: p1 + p2, [extract_two_digits(x) for x in lst])

print(value)