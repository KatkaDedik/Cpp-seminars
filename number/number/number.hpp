#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>
#include <array>
#include <functional>

class integer_number;

std::vector<uint32_t> subtract(const integer_number& first, const integer_number& second);
std::vector<uint32_t> add(const integer_number& first, const integer_number& second);

class integer_number {

	std::vector<uint32_t> data;
	bool sign;
	static const std::array<std::array<std::function<integer_number(const integer_number&, const integer_number&)>, 2>, 2> sign_table;

public:
	integer_number(int num) {
		data.push_back(std::abs(num));
		sign = num < 0;
	}
	integer_number(std::vector<uint32_t> v, bool s) : data(v), sign(s) {}

	size_t size()const { return data.size(); }

	uint32_t get(int index) const {
		if (index >= int(data.size())) {
			return 0;
		}
		return data[index];
	}

	void print() const {
		std::cout << std::hex << data[data.size() - 1] << " ";
		for (int i = data.size() - 2; i >= 0; --i) {
			std::cout << std::setfill('0') << std::setw(8) << std::hex << data[i] << " ";
		}
	}

	static integer_number table_function_nn(const integer_number& first, const integer_number& second) {
		return integer_number(add(first, second), true);
	}
	static integer_number table_function_np(const integer_number& first, const integer_number& second) {
		if (first > second) {
			return integer_number(subtract(first, second), true);
		}
		return integer_number(subtract(second, first), false);
	}
	static integer_number table_function_pn(const integer_number& first, const integer_number& second) {
		if (first < second) {
			return integer_number(subtract(second, first), true);
		}
		return integer_number(subtract(first, second), false);
	}
	static integer_number table_function_pp(const integer_number& first, const integer_number& second) {
		return integer_number(add(first, second), false);
	}

	integer_number operator+(const integer_number& num) const
	{
		return integer_number(sign_table[sign][num.sgn()](*this, num));
	}

	integer_number operator-(const integer_number& num) const
	{
		return integer_number(sign_table[sign][!num.sgn()](*this, num));
	}

	integer_number multiply(uint32_t num, size_t offset) const {

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

	integer_number power(int) const {}

	bool sgn() const { return sign; }

	integer_number operator*(const integer_number& num) const 
	{
		integer_number ret(0);
		for (size_t i = 0; i < num.size(); i++) {
			integer_number temp = multiply(num.get(i), i);
			ret = ret + temp;
		}
		if (sign ^ num.sgn()) { return -ret; }
		return ret;
	}

	/* -1 this < num	0 this == num	1 this > num */
	int cmp(const integer_number& num)const {
		if (!sign && num.sgn()) {
			return 1;
		}
		if (sign && !num.sgn()) {
			return -1;
		}
		if (sign && num.sgn()) {
			if (data.size() > num.size()) {
				return -1;
			}
			for (size_t i = 0; i < data.size(); i++) {
				if (data[i] > num.get(i)) {
					return -1;
				}
				if (data[i] < num.get(i)) {
					return 1;
				}
			}
		}
		if (!sign && !num.sgn()) {
			for (size_t i = 0; i < data.size(); i++) {
				if (data.size() < num.size()) {
					return -1;
				}

				if (data[i] > num.get(i)) {
					return 1;
				}
				if (data[i] < num.get(i)) {
					return -1;
				}
			}
		}
		
		return 0;
	}

	bool operator==(const integer_number& num) const {
		return cmp(num) == 0;
	}
	bool operator!=(const integer_number& num) const {
		return cmp(num) != 0;
	}

	bool operator>(const integer_number& num) const {
		return cmp(num) == 1;
	}

	bool operator>=(const integer_number& num) const { return cmp(num) != -1; }

	bool operator<(const integer_number& num) const {
		return cmp(num) == -1;
	}

	bool operator<=(const integer_number& num) const { return cmp(num) != 1; }

	integer_number& operator-() 
	{ 
		sign = !sign; 
		return *this;
	}

	integer_number devide(const uint32_t& denominator, int offset) const;

	integer_number operator/(const integer_number& denominator) const;

};



class number {

	integer_number numerator;
	integer_number denominator = 1;
	bool sign = true;

public:

	number(int value) : numerator(value) {}

	number operator+(const number& num) const 
	{
		// Todo
		return number(0);
	}

	number operator-(const number& num) const {
		// Todo
		return number(0);
	}

	number operator-() { sign = !sign; }

	number operator*(const number& num) const {
		auto n = numerator * num.get_numerator();
		auto d = numerator * num.get_denominator();
		return number(n, d, true);
	}
	number operator/(const number& num) const {
		auto n = numerator * num.get_denominator();
		auto d = numerator * num.get_numerator();
		return number(n, d, true);
	}

	bool operator==(const number& num) const {}
	bool operator!=(const number& num) const {}
	bool operator>(const number& num) const {}
	bool operator>=(const number& num) const {}
	bool operator<(const number& num) const {}
	bool operator<=(const number& num) const {}

private:

	number(integer_number n, integer_number d, bool s) : numerator(n), denominator(d), sign(s) {}

protected:

	integer_number get_numerator() const { return numerator; }
	integer_number get_denominator() const { return denominator; }
	bool get_sign() const { return sign; }

};