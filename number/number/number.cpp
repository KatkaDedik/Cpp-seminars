#include "number.hpp"

/*---------------------------NO MEMBRE FUNCTIONS---------------------------*/

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
	remove_zeros(ret);
	return ret;
}

void remove_zeros(std::vector<uint32_t>& data) {
	for (size_t i = data.size() - 1; i > 0; --i) {
		if (data[i] != 0) {
			break;
		}
		data.pop_back();
	}
}

/*---------------------------INTEGER_NUMBER---------------------------*/

const std::array<std::array<std::function<integer_number(const integer_number&, const integer_number&)>, 2>, 2>
integer_number::sign_table = { table_function_pp , table_function_pn , table_function_np , table_function_nn };

uint32_t integer_number::get(int index) const {
	if (index >= int(data.size())) {
		return 0;
	}
	return data[index];
}

void integer_number::print() const {
	std::cout << std::hex << data[data.size() - 1] << " ";
	for (int i = data.size() - 2; i >= 0; --i) {
		std::cout << std::setfill('0') << std::setw(8) << std::hex << data[i] << " ";
	}
}

integer_number integer_number::table_function_nn(const integer_number& first, const integer_number& second) {
	return integer_number(add(first, second), true);
}

integer_number integer_number::table_function_np(const integer_number& first, const integer_number& second) {
	if (first > second) {
		return integer_number(subtract(first, second), true);
	}
	return integer_number(subtract(second, first), false);
}

integer_number integer_number::table_function_pn(const integer_number& first, const integer_number& second) {
	if (first < second) {
		return integer_number(subtract(second, first), true);
	}
	return integer_number(subtract(first, second), false);
}

integer_number integer_number::table_function_pp(const integer_number& first, const integer_number& second) {
	return integer_number(add(first, second), false);
}

integer_number integer_number::multiply(uint32_t num, size_t offset) const {

	std::vector<uint32_t> ret;
	uint64_t overflow = 0;

	for (size_t i = 0; i < offset; i++) {
		ret.push_back(0);
	}

	for (size_t i = 0; i < data.size(); i++) {

		uint64_t left = data[i];
		uint64_t right = num;

		uint64_t value = left * right + overflow;
		overflow = value >> 32;
		ret.push_back(value & 0xffffffff);
	}
	if (overflow > 0) {
		ret.push_back(overflow);
	}
	return integer_number(ret, false);
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
	return integer_number(ret , sign);
}

integer_number integer_number::power(int) const 
{ 
	//TODO
	return integer_number(0); 
}

integer_number integer_number::operator+(const integer_number& num) const
{
	return integer_number(sign_table[sign][num.sgn()](*this, num));
}

integer_number integer_number::operator-(const integer_number& num) const
{
	return integer_number(sign_table[sign][!num.sgn()](*this, num));
}

integer_number& integer_number::operator-()
{
	sign = !sign;
	return *this;
}

integer_number integer_number::operator*(const integer_number& num) const
{
	integer_number ret(0);
	for (size_t i = 0; i < num.size(); i++) {
		integer_number temp = multiply(num.get(i), i);
		ret = ret + temp;
	}
	if (sign ^ num.sgn()) { return -ret; }
	return ret;
}

integer_number integer_number::operator/(const integer_number& denominator) const 
{
	integer_number remainder = denominator;
	int offset = denominator.size();
	offset--;
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
	remove_zeros(qoutient.data);
	return integer_number(qoutient);
}

int integer_number::cmp(const integer_number& right)const {
	bool left_sgn = sign;
	bool right_sgn = right.sgn();

	if (data.size() == 1 && data.size() == 1 && data[0] == 0 && right == 0) { return true; }

	if (!left_sgn && right_sgn) {
		return 1;
	}

	if (left_sgn && !right_sgn) {
		return -1;
	}

	//both are negative
	if (left_sgn && right_sgn) { 
		return abs_cmp(right) * -1;
	}

	//both positive
	return abs_cmp(right);
}

int integer_number::abs_cmp(const integer_number& right) const {
	if (data.size() < right.size()) {
		return -1;
	}
	for (size_t i = 0; i < data.size(); i++) {

		if (data[i] > right.get(i)) {
			return 1;
		}
		if (data[i] < right.get(i)) {
			return -1;
		}
	}
	return 0;
}


