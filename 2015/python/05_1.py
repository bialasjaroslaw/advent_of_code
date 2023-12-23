import collections
import itertools

vowels = {'a', 'e', 'i', 'o', 'u'}
bad_seq = {'ab', 'cd', 'pq', 'xy'}

def sliding_window(iterable, n):
    it = iter(iterable)
    window = collections.deque(itertools.islice(it, n-1), maxlen=n)
    for x in it:
        window.append(x)
        yield tuple(window)

def is_nice_string(s: str):
	return sum(map(lambda char: s.count(char), vowels)) >= 3 and \
		not any(map(lambda x: x in s, bad_seq)) and \
		any(map(lambda e: e[0] == e[1], sliding_window(s, 2)))


with open('../input_05_1.txt', 'r') as f:
	print(sum(map(lambda line: is_nice_string(line), f.readlines())))
