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

	bool sgn() const { return sign; }
	size_t size()const { return data.size(); }
	uint32_t get(int index) const;
	void print() const;

	static integer_number table_function_nn(const integer_number& first, const integer_number& second);
	static integer_number table_function_np(const integer_number& first, const integer_number& second);
	static integer_number table_function_pn(const integer_number& first, const integer_number& second);
	static integer_number table_function_pp(const integer_number& first, const integer_number& second);

	integer_number multiply(uint32_t num, size_t offset) const;
	integer_number devide(const uint32_t& denominator, int offset) const;

	integer_number power(int) const;
	integer_number operator+(const integer_number& num) const;
	integer_number operator-(const integer_number& num) const;
	integer_number& operator-();
	integer_number operator*(const integer_number& num) const;
	integer_number operator/(const integer_number& denominator) const;

	/* -1 this < num	0 this == num	1 this > num */
	int cmp(const integer_number& num)const;

	bool operator==(const integer_number& num) const { return cmp(num) == 0; }
	bool operator!=(const integer_number& num) const { return cmp(num) != 0; }
	bool operator>(const integer_number& num) const { return cmp(num) == 1; }
	bool operator>=(const integer_number& num) const { return cmp(num) != -1; }
	bool operator<(const integer_number& num) const { return cmp(num) == -1; }
	bool operator<=(const integer_number& num) const { return cmp(num) != 1; }

private:

	int abs_cmp(const integer_number& right) const;

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