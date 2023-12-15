#include <text_processing/split.hpp>
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
	auto total_area = 0;
	for(const auto& line : lines){
		auto parts = Text::split(line, 'x');
		if(parts.size() != 3){
			continue;
		}
		auto w = std::strtol(parts[0].data(), nullptr, 10);
		auto h = std::strtol(parts[1].data(), nullptr, 10);
		auto l = std::strtol(parts[2].data(), nullptr, 10);

		auto volume = w * h * l;
		auto max_val = w > l ? (w > h ? w : h) : (l > h ? l : h);
		auto add_area = volume / max_val;

		auto area = 2*(w*h + h*l + l*w) + add_area;
		// fmt::print("{}\n", area);
		total_area += area;
	}
	fmt::print("{}\n", total_area);
	
	
	return 0;
}