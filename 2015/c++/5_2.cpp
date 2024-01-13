#include <io_dev/file.h>
#include <fmt/printf.h>
#include <text_processing/count.hpp>

#include <set>
#include <string_view>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/none_of.hpp>
#include <range/v3/view/sliding.hpp>

bool is_nice_line(const std::string& str) {
	if (ranges::none_of(ranges::views::sliding(str, 3), [&](const auto& sub){
		return *sub.begin() == *(sub.begin() + 2);
	}))
		return false;
	if (ranges::none_of(ranges::views::sliding(str, 4), [&](const auto& sub){
		return Text::find(
			str, std::string_view(&*sub.begin(), 2), 
			static_cast<int32_t>(ranges::distance(str.begin(),  sub.begin()) + 2)) != Text::End;
	}))
		return false;
	return true;
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
	fmt::print("{}\n", ranges::count_if(lines, is_nice_line));
	return 0;
}