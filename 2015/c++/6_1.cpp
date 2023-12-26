#include <io_dev/file.h>
#include <fmt/printf.h>

#include <text_processing/convert.hpp>
#include <text_processing/split.hpp>
#include <text_processing/sub.hpp>

#include <string_view>

constexpr bool debug_msg = false;
constexpr bool dump_struct = false;

struct point{
	uint64_t y = 0;
	uint64_t x = 0;
};

struct bounds{
	point start;
	point end;
};

using size = point;

template <typename T>
class grid {
public:
	grid(uint64_t width, uint64_t height, const T& value)
	: _height(height)
	, _width(width)
	, _mem(new T[width * height])
	{
		fill(value);
	}

	void fill(const T& value){
		for(auto* it = _mem.get() ; it != _mem.get() + _width * _height ; ++it)
			*it = value;
	}

	T* ptr(uint64_t row, uint64_t col){
		return _mem.get() + row * _width + col;
	}

	T* ptr(const point& pt){
		return ptr(pt.y, pt.x);
	}

	T& at(uint64_t row, uint64_t col){
		return *ptr(row, col);
	}

	T& at(const point& pt){
		return at(pt.y, pt.x);
	}

	uint64_t width() const{
		return _width;
	}

	uint64_t height() const{
		return _height;
	}

	point start_pt() const{
		return point(0, 0);
	}

	point end_pt() const{
		return point(_height, _width);
	}

	bounds whole() const{
		return bounds{start_pt(), end_pt()};
	}

private:
	uint64_t _width = 0;
	uint64_t _height = 0;
	std::unique_ptr<T[]> _mem;
};


template <typename T>
class grid_iterator{
public:
	grid_iterator(grid<T>& g, point pt, bounds area)
	: _g(g)
	, _ptr(g.ptr(pt))
	, _start(g.ptr(pt.y, area.start.x))
	, _end(g.ptr(pt.y, area.end.x))
	, _stride(g.width())
	{}
	grid_iterator& operator++(){
		if(++_ptr == _end){
			_end += _stride;
			_start += _stride;
			_ptr = _start;
		}
		return *this;
	}
	T& operator*(){
		return *_ptr;
	}
	const T& operator*() const{
		return *_ptr;
	}
	bool operator<=(const grid_iterator& other) const{
		return _ptr <= other._ptr;
	}
	uint64_t stride() const{
		return _stride;
	}

private:
	grid<T>& _g;
	T* _ptr = nullptr;
	T* _start = nullptr;
	T* _end = nullptr;
	point pt;
	uint64_t _stride = 0;
};

template <typename T>
class grid_view{
public:
	grid_view(grid<T>& g) 
	: grid_view(g, g.whole())
	{}

	grid_view(grid<T>& g, bounds b) 
	: _g(g)
	, _b(b)
	, _s(g, b.start, b)
	, _e(g, point{b.end.y - 1, b.end.x - 1}, b)
	{
		if(debug_msg)
			fmt::print("Constructing view with {},{} -> {},{}\n", b.start.y, b.start.x, b.end.y, b.end.x);
	}
	void assign(const T& value){
		if(debug_msg)
			fmt::print("Assign {}, going from {} -> {} ({} elements)\n", value, &*_s - _g.ptr(0, 0), &*_e - _g.ptr(0, 0), &*_e - &*_s);
		for(auto it = _s ; it <= _e ; ++it){
			*it = value;
		}
	}
	void toggle(){
		if(debug_msg)
			fmt::print("Toggle, going from {} -> {} ({} elements)\n", &*_s - _g.ptr(0, 0), &*_e - _g.ptr(0, 0), &*_e - &*_s);
		for(auto it = _s ; it <= _e ; ++it)
			*it = uint64_t{1} -*it;
	}
	template <typename F>
	uint64_t count_if(F&& f){
		uint64_t cnt = 0;
		for(auto it = _s ; it <= _e ; ++it)
			if(f(*it))
				++cnt;
		return cnt;
	}
	void print() const{
		auto line_length = _b.end.x - _b.start.x;
		auto lines = _b.end.y - _b.start.y;
		auto ptr = &*_s;
		auto cnt = 1;
		for(auto it = &*_s ; it <= &*_e ; it += _s.stride()){
			fmt::print("{}\n", fmt::join(std::span(it, line_length), " "));
		}
	}
	grid<T>& _g;
	bounds _b;
	grid_iterator<T> _s;
	grid_iterator<T> _e;
};

struct operation{
	point start;
	point stop;
	uint8_t code = 0;
};

point parse_point(const std::string_view& str){
	auto numbers = Text::convert<uint64_t>(Text::split(str, ","));
	return point{.y = numbers[0], .x = numbers[1]};
}

operation parse_operation(const std::string& str){
	operation op;
	uint64_t start = 0;
	if(str.starts_with("turn on")){
		op.code = 1;
		start = 8;
	}
	else if(str.starts_with("turn off")){
		op.code = 0;
		start = 9;
	}
	else{
		op.code = 2;
		start = 7;
	}
	auto num_values = Text::split(Text::mid_view(str, start), " through ");
	op.start = parse_point(num_values[0]);
	op.stop = parse_point(num_values[1]);
	++op.stop.x;
	++op.stop.y;
	if(op.stop.x == op.start.x)
		++op.stop.x;
	if(op.stop.y == op.start.y)
		++op.stop.y;
	if(debug_msg)
			fmt::print("Op {} pt {},{}, end {},{}\n", op.code, op.start.y, op.start.x, op.stop.y, op.stop.x);
	return op;
}

template <typename T>
void apply_operation(const operation& op, grid<T>& g){
	if(dump_struct)
	{
		auto view = grid_view(g);
		fmt::print("===== BEFORE\n");
		view.print();
		fmt::print("=====\n");
	}
	{
		auto view = grid_view(g, bounds{op.start, op.stop});
		if(dump_struct){
			fmt::print("===== VIEW\n");
			view.print();
			fmt::print("=====\n");
		}
		if(op.code == 2){
			view.toggle();
		}
		else
			view.assign(op.code);
	}
	if(dump_struct)
	{
		auto view = grid_view(g);
		fmt::print("===== AFTER\n");
		view.print();
		fmt::print("=====\n");
	}
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
	grid g(1000, 1000, 0);
	for(const auto& line : lines){
		if(line.size() == 0)
			continue;
		auto op = parse_operation(line);
		apply_operation(op, g);
	}
	fmt::print("{}\n", grid_view(g).count_if([](auto& val){return val > 0;}));
	return 0;
}