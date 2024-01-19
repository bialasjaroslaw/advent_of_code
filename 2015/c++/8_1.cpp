#include <io_dev/file.h>
#include <fmt/printf.h>

uint64_t decode_space(const std::string& str){
	if(str.size() < 2)
		return 0;
	uint64_t len = 0;
	bool sequence = false;
	for(uint64_t idx = 1 ; idx < str.size() - 1 ; ++idx){
		if(str[idx] == '\\'){
			if(sequence)
				++len;
			sequence = !sequence;
		}
		else if(sequence && str[idx] == 'x'){
			sequence = false;
			++len;
			idx += 2;
		}
		else{
			sequence = false;
			++len;
		}
	}

	return len;
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
	auto maybe_lines = IO::File(path, IO::OpenMode::Read).read_lines();
	if(!maybe_lines){
		fmt::print("Can not read lines from {}\n", path.string());
		return -1;
	}
	auto lines = maybe_lines.value();
	auto total = 0UL;
	for(const auto& line : lines){
		auto enc_len = decode_space(line);
		total += (line.size() - enc_len);
	}
	fmt::print("{}\n", total);
	
	return 0;
}