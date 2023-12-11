from typing import List
import functools

class Draw:
	def __init__(self, red: int = 0, green: int = 0, blue: int = 0):
		self._red = red
		self._green = green
		self._blue = blue

	def __repr__(self):
		return f"{{R: {self._red}, G: {self._green}, B: {self._blue}}}"

	def __le__(self, other: 'Draw'):
		return all([getattr(self, field) <= getattr(other, field) for field in ['_red', '_green', '_blue']])

	@classmethod
	def from_str(cls, s: str):
		results = s.split(', ')
		pairs = (result.split(' ') for result in results)
		args = {color: int(num) for num, color in pairs}
		return cls(**args)

class Game:
	def __init__(self, game_id: int, draws: List[Draw]):
		self._draws = draws
		self._game_id = game_id

	def __repr__(self):
		return f"[ID: {self._game_id} - {', '.join([str(draw) for draw in self._draws])}]"

	@classmethod
	def from_str(cls, s: str):
		game_parts = s.split(': ')
		game_id = int(game_parts[0].split('Game ')[1])
		draws = [Draw.from_str(draw) for draw in game_parts[1].split('; ')]
		return cls(game_id, draws)

	def max_color(self, color: str):
		max_value = 0
		for draw in self._draws:
			max_value = max(max_value, getattr(draw, '_' + color))
		return max_value

	def max_draw(self):
		args = {color_name: self.max_color(color_name) for color_name in ['red', 'green', 'blue']}
		return Draw(**args)


	def is_possible(self, draw: Draw):
		m_draw = self.max_draw()
		return draw >= m_draw

	def game_id(self):
		return self._game_id


lines = open('../input_02_1.txt', 'r').read().splitlines()
games = [Game.from_str(line) for line in lines]
new_draw = Draw.from_str('12 red, 13 green, 14 blue')
possible_games = filter(lambda x: x.is_possible(new_draw), games)
game_ids = map(lambda x: x.game_id(), possible_games)
value = functools.reduce(lambda id1, id2: id1 + id2, game_ids)
print(value)