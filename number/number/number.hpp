#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include  <iomanip>

class integer_number {

	std::vector<uint32_t> data;

public:
	integer_number(int num) {
		data.push_back(std::abs(num));
	}
	integer_number(std::vector<uint32_t> v) : data(v) {}

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

	integer_number operator+(const integer_number& num) const
	{
		size_t index = 0;
		std::vector<uint32_t> ret;
		bool carry = 0;
		while (index < data.size() || index < num.size() || carry) {

			uint64_t left = get(index);
			uint64_t right = num.get(index);

			uint64_t value = left + right + carry;

			ret.push_back(value & 0xffffffff);
			carry = value >> 32;
			index++;
		}
		return integer_number(ret);
	}

	integer_number operator-(const integer_number& num) const
	{

		if (*this < num) {
			return integer_number(0);
		}

		size_t index = 0;
		std::vector<uint32_t> ret;
		bool carry = 0;
		while (index < data.size()) {

			uint64_t left = get(index);
			uint64_t right = num.get(index);
			uint64_t value = (left | 0x100000000) - (right + carry);

			ret.push_back(value & 0xffffffff);
			carry = !(value >> 32);
			index++;
		}
		return integer_number(ret);
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
		return ret;
	}

	integer_number operator*(const integer_number& num) const 
	{
		integer_number ret(0);
		for (size_t i = 0; i < num.size(); i++) {
			integer_number temp = multiply(num.get(i), i);
			ret = ret + temp;
		}
		return ret;
	}

	bool operator==(const integer_number& num) const {
		if (data.size() != num.size()) {
			return false;
		}

		for (size_t i = 0; i < data.size(); i++) {
			if (data[i] != num.get(i)) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const integer_number& num) const {
		return !(*this == num);
	}

	bool operator>(const integer_number& num) const {
		if (data.size() < num.size()) {
			return false;
		}
		for (int i = data.size() - 1; i >= 0; i--) {
			if (data[i] == num.get(i)) {
				continue;
			}
			return data[i] > num.get(i);
		}
		return false;
	}

	bool operator>=(const integer_number& num) const { return *this > num || *this == num;  }

	bool operator<(const integer_number& num) const {

		return !(*this >= num);
	}

	bool operator<=(const integer_number& num) const { return !(*this > num); }
};



class number {

	integer_number numerator;
	integer_number denominator = 1;
	bool sign = true;

public:

	number(int value) : numerator(value) {}

	number operator+(const number& num) const 
	{
		if (sign == num.get_sign()) {
			if (denominator != num.get_denominator()) {
				auto n = numerator * num.get_denominator() + denominator * num.get_numerator();
				auto d = denominator * num.get_denominator();
				return number(n, d, sign);
			}
			auto n = numerator + num.get_numerator();
			return number(n, denominator, sign);
		}
		
		//else Todo
		return number(0);
	}

	number operator-(const number& num) const {
		if (denominator != num.get_denominator()) {
			auto n = numerator * num.get_denominator() - denominator * num.get_numerator();
			auto d = denominator * num.get_denominator();
			return number(n, d, true);
		}
		auto n = numerator - num.get_numerator();
		return number(n, denominator, true);
	}

	number operator-() { sign = !sign; }

	number operator*(const number& num) const {
		auto n = numerator * num.get_numerator();
		auto d = numerator * num.get_denominator();
		number(n, d, true);
	}
	number operator/(const number& num) const {
		auto n = numerator * num.get_denominator();
		auto d = numerator * num.get_numerator();
		number(n, d, true);
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

