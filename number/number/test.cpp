#include "number.hpp"
#include <cassert>
#include <iostream>


int main()
{
	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	std::vector<uint32_t> one;
	one.push_back(0x1);
	integer_number long_number(l);
	integer_number number_one(one);

	long_number.print();
	std::cout << " + ";
	number_one.print();
	std::cout << " = ";
	integer_number ll = long_number + number_one;
	ll.print();

	std::vector<uint32_t> vv;
	vv.push_back(0x0);
	vv.push_back(0x1);
	integer_number v(vv);

	std::cout << std::endl;
	v.print();
	std::cout << " - ";
	number_one.print();
	std::cout << " = ";
	integer_number sub = v - number_one;
	sub.print();

	std::cout << std::endl;
	integer_number tmp = long_number * long_number;
	tmp.print();
}