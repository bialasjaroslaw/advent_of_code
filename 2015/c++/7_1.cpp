#include <io_dev/file.h>
#include <fmt/printf.h>

#include <text_processing/convert.hpp>
#include <text_processing/split.hpp>
#include <text_processing/sub.hpp>
#include <text_processing/find.hpp>

#include <range/v3/action/remove_if.hpp>

#include <string_view>
#include <unordered_map>

enum class Operation{
	Equal,
	Not,
	And,
	Or,
	LShift,
	RShift,
	Solved,
	Invalid,
};

std::unordered_map<std::string, uint16_t> solved;

struct Relation{
	std::string lhs;
	std::string rhs;
	Operation op = Operation::Invalid;
	std::string result;
	uint16_t l_value = 0;
	uint16_t r_value = 0;
	bool l_stat = false;
	bool r_stat = false;


	static int32_t convert_or_neg(auto str){
		try{
			return Text::convert<int32_t>(str);
		}
		catch(...){
			return -1;
		}
	}

	static Relation create(const std::string& receipe){
		auto parts = Text::split(receipe, " -> ");
		if(parts.size() != 2)
			throw std::runtime_error("Invalid size");
		Relation rp;
		rp.result = parts[1];
		if(Text::find(parts[0], "NOT ") != Text::End){
			rp.op = Operation::Not;
			rp.lhs = Text::mid(parts[0], 4);
		}
		else if(Text::find(parts[0], " AND ") != Text::End){
			auto operands = Text::split(parts[0], " AND ");
			rp.op = Operation::And;
			if(!rp.set_l(convert_or_neg(operands[0]))){
				rp.lhs = operands[0];
			}
			if(!rp.set_r(convert_or_neg(operands[1]))){
				rp.rhs = operands[1];	
			}
		}
		else if(Text::find(parts[0], " OR ") != Text::End){
			auto operands = Text::split(parts[0], " OR ");
			rp.op = Operation::Or;
			if(!rp.set_l(convert_or_neg(operands[0]))){
				rp.lhs = operands[0];
			}
			if(!rp.set_r(convert_or_neg(operands[1]))){
				rp.rhs = operands[1];	
			}
		}
		else if(Text::find(parts[0], " LSHIFT ") != Text::End){
			auto operands = Text::split(parts[0], " LSHIFT ");
			rp.op = Operation::LShift;
			rp.lhs = operands[0];
			rp.set_r(convert_or_neg(operands[1]));
		}
		else if(Text::find(parts[0], " RSHIFT ") != Text::End){
			auto operands = Text::split(parts[0], " RSHIFT ");
			rp.op = Operation::RShift;
			rp.lhs = operands[0];
			rp.set_r(convert_or_neg(operands[1]));
		}
		else{
			rp.op = Operation::Equal;
			auto val = convert_or_neg(parts[0]);
			if(val >= 0)
				rp.set_l(val);
			rp.lhs = parts[0];
		}
		return rp;
	}

	static bool validate_value(int32_t value){
		return value >= 0 && value <= 65535;
	}

	bool set_l(int32_t val){
		if(Relation::validate_value(val)){
			l_stat = true;
			l_value = static_cast<uint16_t>(val);
			if(op == Operation::Equal){
				op = Operation::Solved;
			}
		}
		return l_stat;
	}
	bool set_r(int32_t val){
		if(Relation::validate_value(val)){
			r_stat = true;
			r_value = static_cast<uint16_t>(val);
		}
		return r_stat;
	}

	bool validate_l(){
		if(!l_stat && solved.contains(lhs)){
			l_stat = true;
			l_value = solved[lhs];
		}
		return l_stat;
	}

	bool validate_r(){
		if(op == Operation::Not || op == Operation::Equal)
			return true;
		if(!r_stat && solved.contains(rhs)){
			r_stat = true;
			r_value = solved[rhs];
		}
		return r_stat;
	}

	bool validate(){
		auto lhs_solvable = validate_l();
		auto rhs_solvable = validate_r();
		return lhs_solvable && rhs_solvable;
	}

	uint16_t solve(){
		uint16_t value = 0;
		if(op == Operation::Not){
			value = ~l_value;
		}
		else if(op == Operation::And){
			value = l_value & r_value;
		}
		else if(op == Operation::Or){
			value = l_value | r_value;
		}
		else if(op == Operation::LShift){
			value = static_cast<uint16_t>(l_value << r_value);
		}
		else if(op == Operation::RShift){
			value = static_cast<uint16_t>(l_value >> r_value);
		}
		else{
			value = l_value;
		}
		op = Operation::Solved;
		return value;
	}
};

std::vector<Relation> ripple;

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
	for(const auto& line : lines){
		if(line.size() == 0)
			continue;
		auto relation = Relation::create(line);
		if(relation.op == Operation::Solved)
			solved[relation.result] = relation.l_value;
		else{
			ripple.push_back(relation);
		}
	}

	bool changed = true;
	while(changed){
		changed = false;
		for(auto& ri : ripple){
			if(ri.validate()){
				solved[ri.result] = ri.solve();
				changed = true;
			}
		}
		ripple = ranges::actions::remove_if(ripple, [](const Relation& r){ return r.op == Operation::Solved; });
	}
	fmt::print("{}\n", solved["a"]);
	return 0;
}