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
	auto maybe_lines = IO::File(path, IO::OpenMode::Read).read_lines();
	if(!maybe_lines){
		fmt::print("Can not read lines from {}\n", path.string());
		return -1;
	}
	auto lines = maybe_lines.value();
	fmt::print("{} lines read\n", lines.size());
	for(const auto& line : lines)
		fmt::print("{}\n", line);
	return 0;
}