nodes = dict()
solutions = dict()

def operand_identity(arg):
	return arg

def operand_not(arg):
	return 65535 - arg

def operand_and(arg1, arg2):
	return arg1 & arg2

def operand_or(arg1, arg2):
	return arg1 | arg2

def operand_lshift(arg1, arg2):
	return arg1 << arg2

def operand_rshift(arg1, arg2):
	return arg1 >> arg2

commands = {'NOT ': operand_not, ' AND ': operand_and, ' OR ': operand_or, ' LSHIFT ': operand_lshift, ' RSHIFT ': operand_rshift}

def try_parse(s: str):
	try:
		return int(s)
	except ValueError:
		return s

def try_parse_list(l: list):
	return [try_parse(item) for item in l if item != '']

def parse_receipe(line: str):
	for operand, fun in commands.items():
		if operand in line:
			return fun, try_parse_list(line.split(operand))
	raise ValueError('line')

def get_key_value(key):
	if type(key) is int:
		return key
	return solutions[key]

with open('../input_07_1.txt', 'r') as f:
	for line in f.read().split('\n'):
		if len(line) == 0:
			break
		parts = line.split(' -> ')
		target = parts[1]
		receipe = parts[0]
		if any([command in receipe for command in commands]):
			operand, keys = parse_receipe(receipe)
			nodes[target] = {'operand': operand, 'keys': keys}
		elif type(try_parse(receipe)) is str:
			nodes[target] = {'operand': operand_identity, 'keys': [receipe]}
		else:
			solutions[target] = int(receipe)

	cnt = 0
	solutions['b'] = 956

	while len(nodes) > 0:
		nodes_to_remove = []
		for node, node_data in nodes.items():
			if all([type(key) is int or key in solutions for key in node_data['keys']]):
				solutions[node] = node_data['operand'](*[get_key_value(key) for key in node_data['keys']])
				nodes_to_remove.append(node)

		for name in nodes_to_remove:
			del nodes[name]

		if cnt > 10000:
			print('Too much iterations')
			break

		cnt += 1

print(solutions['a'])
