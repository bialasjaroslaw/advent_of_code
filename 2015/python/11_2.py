import collections
import itertools

def sliding_window(iterable, n):
    it = iter(iterable)
    window = collections.deque(itertools.islice(it, n-1), maxlen=n)
    for x in it:
        window.append(x)
        yield tuple(window)

def next_str(current: str):
	lst = list(current)[::-1]
	carry = 1
	for i, l in enumerate(lst):
		if carry == 0:
			break
		l = chr(ord(l) + carry)
		carry = 0
		if l == '{':
			carry = 1
			l = 'a'
		lst[i] = l
	return ''.join(lst[::-1])

def sequence(line: str, start: int):
	for idx, w in enumerate(sliding_window(line[start:], 2)):
		if w[0] == w[1]:
			return idx
	return -1

def allowed(passwd: str):
	nums = [ord(ch) for ch in passwd]
	if not any(w[0] + 1 == w[1] and w[1] + 1 == w[2] for w in sliding_window(nums, 3)):
		return False
	if any(let in passwd for let in {'i', 'o', 'l'}):
		return False
	seq_start = sequence(passwd, 0)
	if seq_start == -1:
		return False
	other_seq = -1
	for idx in range(seq_start + 2, len(passwd)):
		if passwd[idx] != passwd[seq_start]:
			other_seq = idx
			break
	if other_seq == -1:
		return False
	other_seq = sequence(passwd, other_seq)
	if other_seq == -1:
		return False
	return True
	

def next_passwd(current: str):
	while True:
		current = next_str(current)
		if allowed(current):
			return current
	return None

with open('../input_11_1.txt', 'r') as f:
	line = f.read().strip()
	prev = next_passwd(line)
	print(next_passwd(prev))
