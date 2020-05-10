#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <stdexcept>
#include <tuple>

class integer_number;

std::vector<uint32_t> subtract(const integer_number& first, const integer_number& second, size_t offset);
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

	static integer_number table_function_nn(const integer_number& first, const integer_number& second); // -  -
	static integer_number table_function_np(const integer_number& first, const integer_number& second); // -  +
	static integer_number table_function_pn(const integer_number& first, const integer_number& second); // +  -
	static integer_number table_function_pp(const integer_number& first, const integer_number& second); // +  +

	integer_number multiply(uint64_t num, size_t offset) const;
	integer_number devide(const uint32_t& denominator, int offset) const;
	std::tuple<integer_number, uint32_t> devide_mod(const uint32_t& denominator, int offset) const;

	integer_number power(uint32_t exponent) const;
	integer_number operator+(const integer_number& num) const;
	integer_number operator-(const integer_number& num) const;
	integer_number operator-();
	integer_number operator*(const integer_number& num) const;
	integer_number operator/(const integer_number& denominator) const;

	/* -1 this < num	0 this == num	1 this > num */
	int cmp(const integer_number& right)const;

	bool operator==(const integer_number& num) const { return cmp(num) == 0; }
	bool operator!=(const integer_number& num) const { return cmp(num) != 0; }
	bool operator>(const integer_number& num) const { return cmp(num) == 1; }
	bool operator>=(const integer_number& num) const { return cmp(num) != -1; }
	bool operator<(const integer_number& num) const { return cmp(num) == -1; }
	bool operator<=(const integer_number& num) const { return cmp(num) != 1; }
	integer_number& operator=(const std::vector<uint32_t>& vec);
	void change_sign();
	integer_number get_elements(size_t offset, size_t count) const;
	integer_number mod(const integer_number& num) const;
	integer_number gcd(integer_number num) const;
	integer_number& push_forward(const uint32_t& in);

private:

	int abs_cmp(const integer_number& right) const;

};



class number {

	integer_number numerator;
	integer_number denominator = 1;

public:

	number(int value) : numerator(value) {}
	number() : numerator(0) {}

	number operator+(const number& num) const;
	number operator-(const number& num) const;
	number operator-();

	number operator*(const number& num) const;
	number operator/(const number& num) const;

	bool operator==(const number& num) const;
	bool operator!=(const number& num) const;
	bool operator>(const number& num) const;
	bool operator>=(const number& num) const;
	bool operator<(const number& num) const;
	bool operator<=(const number& num) const;

	number power(int exponent) const;
	number sqrt(int precision) const;
	void simplify_sign();
	bool sgn() const;
	void print() const;
	number& simplify();


private:

	number(integer_number n, integer_number d) : numerator(n), denominator(d) {}
	int cmp(const number& num) const;
	number& abs();
	

protected:

	integer_number get_numerator() const { return numerator; }
	integer_number get_denominator() const { return denominator; }
};