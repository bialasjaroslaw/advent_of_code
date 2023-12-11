#include <text_processing/count.hpp>
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
	auto maybe_line = IO::File(path, IO::OpenMode::Read).read();
	if(!maybe_line){
		fmt::print("Can not read lines from {}\n", path.string());
		return -1;
	}
	auto line = maybe_line.value();
	auto floor = Text::count(line, '(') - Text::count(line, ')');
	fmt::print("Floor is {}\n", floor);
	
	return 0;
}