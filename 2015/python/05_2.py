import collections
import itertools

def sliding_window(iterable, n):
    it = iter(iterable)
    window = collections.deque(itertools.islice(it, n-1), maxlen=n)
    for x in it:
        window.append(x)
        yield tuple(window)

def is_nice_string(s: str):
	return any(map(lambda e: e[0] == e[2], sliding_window(s, 3))) and \
		any(map(lambda pair: ''.join(pair[1]) in s[pair[0] + 2:], enumerate(sliding_window(s, 2))))


with open('../input_05_1.txt', 'r') as f:
	print(sum(map(lambda line: is_nice_string(line), f.readlines())))
