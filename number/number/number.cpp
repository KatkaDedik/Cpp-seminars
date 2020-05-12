#include "number.hpp"

/*---------------------------NO MEMBRE FUNCTIONS---------------------------*/

void remove_zeros(std::vector<uint32_t>& data) {
	for (size_t i = data.size() - 1; i > 0; --i) {
		if (data[i] != 0) {
			break;
		}
		data.pop_back();
	}
}

std::vector<uint32_t> add(const integer_number& first, const integer_number& second)
{
	size_t index = 0;
	std::vector<uint32_t> ret;
	bool carry = false;
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

std::vector<uint32_t> subtract(const integer_number& first, const integer_number& second, size_t offset = 0)
{
	size_t index = 0;
	std::vector<uint32_t> ret;
	bool carry = false;
	while (index + offset< first.size()) {

		uint64_t left = first.get(index);
		uint64_t right = second.get(index + offset);
		uint64_t value = (left | 0x100000000) - (right + carry);

		ret.push_back(value & 0xffffffff);
		carry = !(value >> 32);
		index++;
	}
	remove_zeros(ret);
	return ret;
}


/*---------------------------INTEGER_NUMBER---------------------------*/

const std::array<std::array<std::function<integer_number(const integer_number&, const integer_number&)>, 2>, 2>
integer_number::sign_table = {{
	{{table_function_pp , table_function_pn}},
	{{table_function_np , table_function_nn}}
	}};

uint32_t integer_number::get(int index) const {
	if (index >= int(data.size())) {
		return 0;
	}
	return data[index];
}

void integer_number::print() const {
	std::cout  << data[data.size() - 1] << " ";
	for (int i = data.size() - 2; i >= 0; --i) {
		std::cout << data[i] << " ";
	}
}

integer_number integer_number::table_function_nn(const integer_number& first, const integer_number& second) 
{
	// both negative, -first -second = -(first + second)
	return integer_number(add(first, second), true);
}

integer_number integer_number::table_function_np(const integer_number& first, const integer_number& second) 
{
	// first negativ, second positive, -first +second 
	if (first.abs_cmp(second) == 1) {
		return integer_number(subtract(first, second), true);	// |first| > |second| -> -(first - second)
	}
	return integer_number(subtract(second, first), false);		// |first| <= |second| -> +(second - first)
}

integer_number integer_number::table_function_pn(const integer_number& first, const integer_number& second) 
{
	// first positive, second negativ, +first -second 
	if (first.abs_cmp(second) != -1 ) {
		return integer_number(subtract(first, second), false);	// |first| >= |second| -> +(first - second) 
	}
	return integer_number(subtract(second, first), true);		// |first| < |second| -> -(second - first)
}

integer_number integer_number::table_function_pp(const integer_number& first, const integer_number& second) 
{
	// both positive, first + second = +(first + second)
	return integer_number(add(first, second), false);
}

integer_number integer_number::multiply(uint64_t right, size_t offset) const {

	std::vector<uint32_t> ret;
	uint64_t overflow = 0;

	for (size_t i = 0; i < offset; i++) {
		ret.push_back(0);
	}

	for (uint64_t left : data) {
		uint64_t value = left * right + overflow;
		overflow = value >> 32;
		ret.push_back(value & 0xffffffff);
	}
	if (overflow > 0) {
		ret.push_back(overflow & 0xffffffff);
	}
	return integer_number(ret, false);
}

integer_number integer_number::devide(const uint32_t& denominator, int offset) const
{
	uint64_t remainder = 0;
	auto denominator64 = static_cast<uint64_t>(denominator);
	
	if (denominator64 == 0) {
		throw std::logic_error("invalid devide by 0!");
	}

	std::vector<uint32_t> ret;
	for (int i = data.size() - 1; i >= offset; i--) {
		
		remainder |= data[i];
		uint64_t tmp = remainder / denominator64;
		ret.push_back(static_cast<uint32_t>(tmp & 0xffffffff));
		remainder = (remainder % denominator64) << 32;
	}
	std::reverse(std::begin(ret), std::end(ret));
	return integer_number(ret , sign);
}

std::tuple<integer_number, integer_number> integer_number::devide_mod(const integer_number& denominator) const
{
	integer_number numerator = *this;
	uint64_t numerator_tmp = 0;
	const uint64_t denominator_tmp = denominator.get(denominator.size() - 1);
	integer_number numerator_upper_elements = numerator.get_elements(numerator.size(), denominator.size() - 1);
	std::vector<uint32_t> ret;

	for (int i = static_cast<int>(numerator.size()) - 1; i >= static_cast<int>(denominator.size()) - 1; i--) {

		numerator_upper_elements.push_forward(get(i - (denominator.size() - 1)));
		uint64_t tmp = numerator_upper_elements.get(denominator.size());
		numerator_tmp = (tmp << 32) | numerator_upper_elements.get(denominator.size() - 1);

		uint64_t upper_guess = numerator_tmp / denominator_tmp;
		integer_number upper_x_denominator = denominator.multiply(upper_guess, 0);

		integer_number upper_remainder = numerator_upper_elements - upper_x_denominator;

		if (!upper_remainder.sgn()) {

			numerator_upper_elements = upper_remainder;
			ret.push_back(upper_guess);

		}
		else {
			uint64_t lower_guess = numerator_tmp / (denominator_tmp + 1);
			integer_number lower_x_denominator = denominator.multiply(lower_guess, 0);

			integer_number lower_remainder = numerator_upper_elements - lower_x_denominator;

			while (lower_remainder.abs_cmp(denominator) != -1) {

				numerator_tmp = lower_remainder.get(lower_remainder.size() - 1);

				upper_guess = numerator_tmp / denominator_tmp;
				upper_remainder = lower_remainder - denominator.multiply(upper_guess, 0);

				if (!upper_remainder.sgn()) {
					lower_guess += upper_guess;
					lower_remainder = upper_remainder;
				}
				else {
					uint32_t lower_guess2 = numerator_tmp / (denominator_tmp + 1);
					lower_remainder = lower_remainder - denominator.multiply(lower_guess2, 0);
					lower_guess += lower_guess2;
				}
			}

			numerator_upper_elements = lower_remainder;
			ret.push_back(lower_guess);
		}
	}
	std::reverse(std::begin(ret), std::end(ret));
	remove_zeros(ret);
	integer_number res(ret, sign ^ denominator.sgn());
	return std::make_pair(res, numerator_upper_elements);
}

integer_number integer_number::power(uint32_t exponent) const 
{ 
	bool neg = exponent & 0x1;
	if (exponent == 0) { return integer_number(1); }

	integer_number tmp = *this;
	if (tmp.sgn()) { tmp = -tmp; }
	integer_number res(1);
	while (exponent != 0) {
		if (exponent & 0x1) {
			res = res * tmp;
		}
		exponent >>= 1;
		tmp = tmp * tmp;
	}

	if (sign && neg) { return -res; }
	return res; 
}

int integer_number::cmp(const integer_number& right)const {
	bool left_sgn = sign;
	bool right_sgn = right.sgn();

	if (data.size() == 1 && right.size() == 1 && data[0] == 0 && right.get(0) == 0) { return 0; }

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

integer_number integer_number::operator+(const integer_number& num) const
{
	return integer_number(sign_table[sign][num.sgn()](*this, num));
}

integer_number integer_number::operator-(const integer_number& num) const
{
	return integer_number(sign_table[sign][!num.sgn()](*this, num));
}

integer_number integer_number::operator-()
{
	return integer_number(data, !sign);
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
	return std::get<0>(devide_mod(denominator));
}

integer_number& integer_number::push_forward(const uint32_t& in) {
	data.insert(data.begin(), in);
	return *this;
}

integer_number& integer_number::operator=(const std::vector<uint32_t>& vec) 
{ 
	data = vec; 
	return *this; 
}

int integer_number::abs_cmp(const integer_number& right) const {
	if (data.size() < right.size()) {
		return -1;
	}
	for (int i = static_cast<int>(data.size()) - 1; i >= 0 ; i--) {

		if (data[i] > right.get(i)) {
			return 1;
		}
		if (data[i] < right.get(i)) {
			return -1;
		}
	}
	return 0;
}

void integer_number::change_sign() {
	sign = !sign;
}

integer_number integer_number::get_elements(size_t offset, size_t count) const {
	/*if (offset + count >= data.size()) {
		return integer_number(0);
	}*/
	return integer_number(std::vector<uint32_t>(data.begin() + offset - count, data.begin() + offset), false);
}

integer_number integer_number::mod(const integer_number& num) const 
{ 
	return std::get<1>(devide_mod(num)); 
}

integer_number integer_number::gcd(integer_number v) const {
	integer_number zero(0);
	integer_number u = *this;
	while (v != zero) {
		integer_number tmp = u;
		u = v;
		v = tmp.mod(v);
	}
	return u;
}

/*---------------------------NUMBER---------------------------*/

number number::operator+(const number& num) const {
	if (denominator == num.get_denominator()) {
		integer_number new_numerator = numerator + num.get_numerator();
		return number(new_numerator, denominator);
	}
	number ret((numerator * num.get_denominator()) + (num.get_numerator() * denominator)
		, denominator * num.get_denominator());
	ret.simplify();
	return ret;
}

number number::operator-(const number& num) const {
	if (denominator == num.get_denominator()) {
		integer_number new_numerator = numerator - num.get_numerator();
		return number(new_numerator, denominator);
	}
	integer_number new_numerator = (numerator * num.get_denominator()) - (num.get_numerator() * denominator);
	
	number ret((numerator * num.get_denominator()) - (num.get_numerator() * denominator)
		, denominator * num.get_denominator());
	ret.simplify();
	return ret;
}

number number::operator-()
{ 
	if (denominator.sgn()) {
		return number(numerator, -denominator);
	}
	return number(-numerator, denominator);
}

number number::operator*(const number& num) const 
{ 
	if (num == number(0)) { 
		return number(0); 
	}
	if (num == number(1)) { 
		return *this; 
	}

	number ret(numerator * num.get_numerator(), denominator * num.get_denominator());
	return ret.simplify();
}

number number::operator/(const number& num) const 
{ 
	if (num == number(0)) { throw std::logic_error("invalid devide by 0!"); }
	if (num == number(1)) { return *this; }
	number ret(numerator * num.get_denominator(), denominator * num.get_numerator());
	return ret.simplify();
}

bool number::operator==(const number& num) const { return cmp(num) == 0; }
bool number::operator!=(const number& num) const { return cmp(num) != 0; }
bool number::operator>(const number& num) const { return cmp(num) == 1; }
bool number::operator>=(const number& num) const { return cmp(num) != -1; }
bool number::operator<(const number& num) const { return cmp(num) == -1; }
bool number::operator<=(const number& num) const { return cmp(num) != 1; }

number number::power(int exponent) const 

{ 
	if (exponent == 0) { return number(1); }
	if (exponent == 1) { return *this; }
	if (exponent == -1) {
		if (numerator == integer_number(0)) {
			throw std::logic_error("invalid devide by 0!");
		}
		return number(denominator, numerator);
	}
	if (exponent < 0) { return number(denominator.power(std::abs(exponent)), numerator.power(std::abs(exponent))); }
	return number(numerator.power(std::abs(exponent)), denominator.power(std::abs(exponent)));
}

number number::sqrt(int precision) const 
{ 
/*
*https://cs.stackexchange.com/questions/37596/arbitrary-precision-integer-square-root-algorithm*/

	if (sgn()) { 
		throw std::logic_error("invalid sqrt of negateve number!"); 
	}
	number two(2);
	number current;
	number next = *this;
	number tmp;
	number precision_number = number(10).power(-precision);
	do {
		current = next;
		next = current / two + *this / (two * current);
		tmp = (current - next).abs();
	} while ( tmp > precision_number);
	return next; 
}

number& number::abs() 
{
	if (numerator.sgn()) { numerator.change_sign(); }
	if (denominator.sgn()) { denominator.change_sign(); }
	return *this;
}
void number::simplify_sign() 
{
	if (((numerator.sgn() && denominator.sgn())
		|| (!numerator.sgn() && denominator.sgn()))) {
		numerator.change_sign();
		denominator.change_sign();
	}
}
bool number::sgn() const {
	return (numerator.sgn() ^ denominator.sgn());
}
int number::cmp(const number& num) const {

	bool first_sgn = sgn();
	bool second_sgn = num.sgn();

	if (!first_sgn && second_sgn) { // +first -second => first > second
		return 1;
	}

	if (first_sgn && !second_sgn) { // -first +second => first < second
		return -1;
	}

	if (denominator == num.get_denominator()) {
		return numerator.cmp(num.get_numerator());
	}

	return (numerator * num.get_denominator()).cmp(num.get_numerator() * denominator);
}
void number::print() const {
	numerator.print();
	size_t s = std::max(numerator.size(), denominator.size());
	std::cout << std::endl;
	for (; s > 0; s--) {
		std::cout << "--------";
	}
	std::cout << std::endl;
	denominator.print();
	std::cout << std::endl;
	std::cout << std::endl;
}

number& number::simplify() { 
	
	integer_number g_c_d;

	simplify_sign();

	if(numerator.abs_cmp(denominator) == 1){
		g_c_d = numerator.gcd(denominator);
	}
	else {
		g_c_d = denominator.gcd(numerator);
	}

	numerator = numerator / g_c_d;
	denominator = denominator / g_c_d;
	
	return *this; 
}