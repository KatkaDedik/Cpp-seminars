#include "number.hpp"

const std::array<std::array<std::function<integer_number(const integer_number&, const integer_number&)>, 2>, 2>
integer_number::sign_table = { table_function_nn , table_function_np , table_function_pn , table_function_pp };


std::vector<uint32_t> add(const integer_number& first, const integer_number& second)
{
	size_t index = 0;
	std::vector<uint32_t> ret;
	bool carry = 0;
	while (index < first.size() || index < second.size() || carry) {

		uint64_t left = first.get(index);
		uint64_t right = second.get(index);

		uint64_t value = left + right + carry;

		ret.push_back(value & 0xffffffff);
		carry = value >> 32;
		index++;
	}
	return ret;
}


std::vector<uint32_t> subtract(const integer_number& first, const integer_number& second)
{
	size_t index = 0;
	std::vector<uint32_t> ret;
	bool carry = 0;
	while (index < first.size()) {

		uint64_t left = first.get(index);
		uint64_t right = second.get(index);
		uint64_t value = (left | 0x100000000) - (right + carry);

		ret.push_back(value & 0xffffffff);
		carry = !(value >> 32);
		index++;
	}
	return ret;
}