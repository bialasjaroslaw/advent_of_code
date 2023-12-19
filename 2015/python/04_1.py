import hashlib
import itertools

with open('../input_04_1.txt', 'r') as f:
	text = f.read()
	for cnt in itertools.count():
		val = f"{text}{cnt}"
		hash = hashlib.md5(val.encode()).digest()
		if hash[0] == 0 and hash[1] == 0 and hash[2] & 0xF0 == 0:
			print(cnt)
			break


