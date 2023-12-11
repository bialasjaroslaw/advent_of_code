#include <io_device/file_wrapper.h>
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
	using namespace Helpers;
	auto lines = FileWrapper(path, IOMode::Read).read_lines();
	fmt::print("{} lines read\n", lines.size());
	for(const auto& line : lines)
		fmt::print("{}\n", line);
	return 0;
}