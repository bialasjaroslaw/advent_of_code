from typing import List, Tuple
import functools
import copy
import os

class EnginePart:
	def __init__(self, position: Tuple[int, int], part_id: int, length: int):
		self._position = position
		self._part_id = part_id
		self._valid = False
		self._length = length

	def set_valid(self, valid: bool):
		self._valid = valid

	def __repr__(self):
		return f'POS: {self._position}, LEN: {self._length}, VAL: {self._valid}, ID: {self._part_id}'

class EngineSchema:
	def __init__(self, lines: List[str]):
		self._lines = lines
		self._parts = EngineSchema.find_parts(self._lines)
		self.validate_parts()

	def roi(self, pos: Tuple[int, int], length: int) -> Tuple[int, int, int, int]:
		return (max(0, pos[0] - 1), min(len(self._lines), pos[0] + 2), 
				max(0, pos[1] - 1), min(len(self._lines[0]), pos[1] + length + 1))

	def small_roi(self, pos: Tuple[int, int], length: int) -> Tuple[int, int, int, int]:
		return (max(0, pos[0]), min(len(self._lines), pos[0] + 1), 
				max(0, pos[1]), min(len(self._lines[0]), pos[1] + length))

	def contains_symbol(self, roi: Tuple[int, int, int, int]):
		for line_idx in range(roi[0], roi[1]):
			for character_idx in range(roi[2], roi[3]):
				character = self._lines[line_idx][character_idx]
				if not character.isdigit() and character != '.':
					return True
		return False

	def validate_parts(self):
		for part in self._parts:
			roi = self.roi(part._position, part._length)
			part.set_valid(self.contains_symbol(roi))


	@staticmethod
	def parts_in_line(line: str, line_number: int):
		parts = []
		part_start = -1
		for idx, character in enumerate(line):
			is_dig = character.isdigit()
			if is_dig and part_start == -1:
				part_start = idx
			elif not is_dig and part_start != -1:
				part = EnginePart((line_number, part_start), int(line[part_start:idx]), idx-part_start)
				parts.append(part)
				part_start = -1
		else:
			if part_start != -1:
				part = EnginePart((line_number, part_start), int(line[part_start:idx+1]), idx-part_start+1)
				parts.append(part)
				part_start = -1
		return parts


	@staticmethod
	def find_parts(lines: List[str]):
		parts: List[EnginePart] = []
		for idx, line in enumerate(lines):
			parts += EngineSchema.parts_in_line(line, idx)
		return parts

	def __repr__(self):
		return f'[{", ".join([repr(part) for part in self._parts])}]'

	def invalid_parts(self):
		return list(filter(lambda x: not x._valid, self._parts))

	def valid_parts(self):
		return list(filter(lambda x: x._valid, self._parts))


schema = EngineSchema(open('../input_03_1.txt', 'r').read().split('\n'))
invalid_ones = schema.invalid_parts()
valid_ones = schema.valid_parts()
part_ids = map(lambda x: x._part_id, valid_ones)
value = functools.reduce(lambda id1, id2: id1 + id2, part_ids)
print(value)
