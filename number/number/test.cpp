#include "number.hpp"
#include <cassert>
#include <iostream>


void primitive_test() {

	integer_number one(1);
	integer_number two(2);
	integer_number ntwo(-2);
	integer_number none(-1);
	integer_number nthree(-3);

	assert(one + one == two);
	assert(two - one == one);
	assert(one - two == none);
	assert(ntwo + none == nthree);
	assert(ntwo - nthree == one);
	assert(nthree + one == ntwo);
}

int main()
{
	primitive_test();

	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	std::vector<uint32_t> one;
	one.push_back(0x1);
	integer_number long_number(l, true);
	integer_number number_one(one, true);

	long_number.print();
	std::cout << " + ";
	number_one.print();
	std::cout << " = ";
	integer_number ll = long_number + number_one;
	ll.print();

	std::vector<uint32_t> vv;
	vv.push_back(0x0);
	vv.push_back(0x1);
	integer_number v(vv, true);

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