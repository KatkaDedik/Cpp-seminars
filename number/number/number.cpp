#include "number.hpp"

const std::array<std::array<std::function<integer_number(const integer_number&, const integer_number&)>, 2>, 2>
integer_number::sign_table = { table_function_pp , table_function_pn , table_function_np , table_function_nn };


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


integer_number integer_number::devide(const uint32_t& denominator, int offset) const
{
	uint64_t remainder = 0;
	std::vector<uint32_t> ret;
	for (int i = data.size() - 1; i >= offset; i--) {
		
		remainder |= data[i];
		ret.push_back(remainder / static_cast<uint64_t>(denominator));
		remainder = (remainder % denominator) << 32;
	}
	std::reverse(std::begin(ret), std::end(ret));
	return integer_number(ret , false);
}


integer_number integer_number::operator/(const integer_number& denominator) const 
{
	integer_number remainder = denominator;
	int offset = data.size() - 1;
	uint32_t quick_divisor = denominator.get(offset);
	integer_number qoutient = devide(quick_divisor, offset);

	while (remainder >= denominator) {
		remainder = *this - (qoutient * denominator);
		integer_number qoutient_n = qoutient + remainder.devide(quick_divisor, offset);
		qoutient = (qoutient + qoutient_n).devide(2, 0);
	}
	remainder = *this - (qoutient * denominator);
	if ((sign ^ denominator.sgn())) {
		-qoutient;
	}
	return integer_number(qoutient);
}