import json

def count(obj) -> int:
	if type(obj) == dict:
		if "red" in obj.values():
			return 0
		return sum([count(value) for value in obj.values()])
	elif type(obj) == list:
		return sum([count(value) for value in obj])
	elif type(obj) == float or type(obj) == int:
		return obj
	return 0

with open('../input_12_1.txt') as f:
	print(count(json.load(f)))

