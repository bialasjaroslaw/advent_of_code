#include <io_dev/file.h>
#include <fmt/printf.h>

#include <unordered_map>

int32_t sub_to_idx(int32_t y, int32_t x)
{
	return 8192 * y + x;
}

void advance(char c, int& pos_y, int& pos_x)
{
	if(c == '<')
		--pos_x;
	else if(c == '>')
		++pos_x;
	else if(c == '^')
		++pos_y;
	else //if(c == 'v')
		--pos_y;

}

int main(int argc, char const *argv[])
{
	if(argc < 2){
		fmt::print("Not enough arguments\n");
		return 1;
	}
	auto path = std::filesystem::path(argv[1]);
	if(!std::filesystem::exists(path)){
		fmt::print("File not exists\n");
		return 2;
	}
	auto maybe_line = IO::File(path, IO::OpenMode::Read).read();
	if(!maybe_line){
		fmt::print("Can not read lines from {}\n", path.string());
		return -1;
	}
	auto line = maybe_line.value();
	std::unordered_map<int32_t, int32_t> house_map;

	int32_t pos_y = 0;
	int32_t pos_x = 0;

	int32_t robo_pos_y = 0;
	int32_t robo_pos_x = 0;

	++house_map[sub_to_idx(pos_y, pos_x)];
	++house_map[sub_to_idx(robo_pos_y, robo_pos_x)];

	for(uint32_t idx = 0 ; idx < line.size() ; idx+=2)
	{
		auto move = line[idx];
		auto robo_move = line[idx + 1];

		advance(move, pos_y, pos_x);
		advance(robo_move, robo_pos_y, robo_pos_x);
		++house_map[sub_to_idx(pos_y, pos_x)];
		++house_map[sub_to_idx(robo_pos_y, robo_pos_x)];
	}

	auto house_with_presents = 0;
	for(auto&[k, v] : house_map)
		if(v != 0)
			++house_with_presents;

	fmt::print("{}\n", house_with_presents);
	
	return 0;
}