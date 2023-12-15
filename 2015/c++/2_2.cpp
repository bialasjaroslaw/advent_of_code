#include <text_processing/split.hpp>
#include <text_processing/convert.hpp>
#include <io_dev/file.h>
#include <fmt/printf.h>

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
	auto maybe_line = IO::File(path, IO::OpenMode::Read).read_lines();
	if(!maybe_line){
		fmt::print("Can not read lines from {}\n", path.string());
		return -1;
	}
	auto lines = maybe_line.value();
	auto total_length = 0;
	for(const auto& line : lines){
		auto parts = Text::split(line, 'x');
		if(parts.size() != 3){
			continue;
		}
		auto numbers = Text::convert<int32_t>(parts);
		auto w = numbers[0];
		auto h = numbers[1];
		auto l = numbers[2];

		auto max_val = w > l ? (w > h ? w : h) : (l > h ? l : h);
		auto ribbon = 2 * (w + h + l - max_val) + w * h * l;

		total_length += ribbon;
	}
	fmt::print("{}\n", total_length);
	
	
	return 0;
}