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

void less_primitive_test() {

	std::vector<uint32_t> s;
	s.push_back(0xE);
	s.push_back(1);

	integer_number small(s, false);


	std::vector<uint32_t> l;
	l.push_back(0xffffffff);

	integer_number big(l, false);

	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	integer_number large(l, false);
	
	integer_number nlarge(l, true);

	l.push_back(0xffffffff);

	integer_number very_large(l, false);

	integer_number hentai(69);
	integer_number weed(-420);
	integer_number star_wars(54);

	//assert(large * nlarge < weed);
	//assert(big * hentai > large);
	//assert((big - small) * weed < 0);
}

void simple_devide() {

	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	integer_number long_number(l, false);

	std::vector<uint32_t> e;
	e.push_back(0x8ce602fb);
	e.push_back(0x1054d);
	integer_number expected(e, false);

	assert(long_number.devide(0xface, 0) == expected);
	
	l.push_back(0xffffffff);
	integer_number very_long_number(l, false);
	
	e[0] = 0x80000000;
	e[1] = 0x8ce602fb;
	integer_number expected2(e, false);

	assert(long_number.devide(0xface, 0) == expected);
}


int main()
{
	less_primitive_test();
	primitive_test();
	simple_devide();

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