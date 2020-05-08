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

void primitive_devide() {

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

void simple_devide() {

	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	//l.push_back(0xffffffff);
	integer_number long_number(l, false);

	std::vector<uint32_t> osem;
	osem.push_back(0x88888888);
	osem.push_back(0x88888888);
	integer_number osmicky(osem, false);
	
	std::vector<uint32_t> e;
	//e.push_back(0x00000000);
	e.push_back(0xe0000000);
	e.push_back(0x10000000);
	integer_number expected(e, false);
	//assert(long_number / osmicky == expected);

	integer_number res2 = long_number.devide(2, 0);
	assert(long_number / res2 == integer_number(2));

	integer_number res8888 = long_number.devide(0x8888, 0);
	assert(long_number / res8888 == integer_number(0x8888));


	std::vector<uint32_t> smth;
	smth.push_back(0xff);
	smth.push_back(0xffffffff);
	smth.push_back(0xffffffff);
	smth.push_back(0xffffffff);
	smth.push_back(0xffffffff);
	integer_number smth_big(smth, false);
	integer_number resff = smth_big.devide(0xff, 0);
	assert(smth_big / resff == integer_number(0xff));
	
	integer_number hentai(69);

	integer_number r = smth_big + hentai;
	assert(smth_big / resff == integer_number(0xff));

	integer_number vn(999999999);
	integer_number vd(56789);
	assert(vn / vd == integer_number(17609));

	integer_number video_numerator = integer_number(999999999) * integer_number(100000000);
	integer_number video_denominator = integer_number(56789) * integer_number(100000000);
	assert(video_numerator / video_denominator == integer_number(17609));

}


int main()
{
	less_primitive_test();
	primitive_test();
	primitive_devide();
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