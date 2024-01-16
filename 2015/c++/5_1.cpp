#include <io_dev/file.h>
#include <fmt/printf.h>
#include <text_processing/count.hpp>

#include <set>
#include <string_view>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/none_of.hpp>
#include <range/v3/view/sliding.hpp>

std::set<char> vowels{'a', 'e', 'i', 'o', 'u'};
std::set<std::string_view> bad_seq{"ab", "cd", "pq", "xy"};

bool is_nice_line(const std::string& str) {
	if (Text::count_if(str, [&](const char& c){
		return vowels.contains(c);
	}) < 3)
		return false;
	// seems the same as below, but might be slower
	// if (ranges::any_of(bad_seq, [&](const std::string_view& seq){return Text::find(str, seq) != Text::End;}))
	// 	return false;
	if (ranges::any_of(ranges::views::sliding(str, 2), [&](const auto& sub){
		return bad_seq.contains(std::string_view(&*sub.begin(), static_cast<uint32_t>(ranges::distance(sub))));
	}))
		return false;
	if (ranges::none_of(ranges::views::sliding(str, 2), [&](const auto& sub){
		return *sub.begin() == *(sub.begin() + 1);
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