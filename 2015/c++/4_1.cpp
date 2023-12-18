#include <text_processing/split.hpp>
#include <io_dev/file.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <hash/md5.h>

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
	uint8_t output[16];
	int32_t counter = 0;
	while(true)
	{
		auto modified = fmt::format("{}{}", line, counter);
		md5_hasher::from_string(modified.c_str(), output);
		if(output[0] == 0 && output[1] == 0 && (output[2] & 0xF0) == 0)
		{
			fmt::print("{}\n", counter);
			break;
		}
		++counter;
	}
	return 0;
}