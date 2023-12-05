import functools
from typing import Optional, Set

digit_map = {
	'one': 1,
	'two': 2,
	'three': 3,
	'four': 4,
	'five': 5,
	'six': 6,
	'seven': 7,
	'eight': 8,
	'nine': 9,
}

three_digit = {'one', 'two', 'six'}
four_digit = {'four', 'five', 'nine'}
five_digit = {'three', 'seven', 'eight'}

def is_digit_from_on(max_substr: int, s: str, idx: int, sub_len: int, sub_set: Set[str]) -> Optional[int]:
	if max_substr < sub_len:
		return None
	sub_str = s[idx: idx + sub_len]
	if sub_str in sub_set:
		return digit_map[sub_str]

def digit_on(s: str, idx: int) -> Optional[int]:
	max_substr = len(s) - idx

	return is_digit_from_on(max_substr, s, idx, 3, three_digit) or \
			is_digit_from_on(max_substr, s, idx, 4, four_digit) or \
			is_digit_from_on(max_substr, s, idx, 5, five_digit)


def left_digit(s: str) -> int:
	for idx in range(len(s)):
		if s[idx].isdigit():
			return int(s[idx])
		int_digit = digit_on(s, idx)
		if int_digit is not None:
			return int_digit
	return 0

def right_digit(s: str) -> int:
	for idx in range(len(s) - 1, -1, -1):
		if s[idx].isdigit():
			return int(s[idx])
		txt_digit = digit_on(s, idx)
		if txt_digit is not None:
			return int(txt_digit)
	return 0

def number_from_str(s: str) -> int:
	return 10 * left_digit(s) + right_digit(s)

lst = open('../input_01_1.txt').readlines()

value = functools.reduce(lambda p1, p2: p1 + p2, [number_from_str(x) for x in lst])

print(value)