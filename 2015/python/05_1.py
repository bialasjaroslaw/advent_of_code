import string

vowels = {'a', 'e', 'i', 'o', 'u'}

def is_nice_string(s: str):
	occurences = {char: s.count(char) for char in string.ascii_lowercase} 
	sum = 0
	for vowel in vowels:
		sum += occurences[vowel]
	if sum < 3:
		return False
	have_double = False
	for idx in range(len(s) - 1):
		if s[idx+1] == s[idx]:
			have_double = True
			break
	if not have_double:
		return False

	if 'ab' in s or 'cd' in s or 'pq' in s or 'xy' in s:
		return False
	return True


with open('../input_05_1.txt', 'r') as f:
	nice = 0
	for line in f.readlines():
		if is_nice_string(line):
			nice += 1
	print(nice)

